/*
* Copyright 2017-2023 NVIDIA Corporation.  All rights reserved.
*
* Please refer to the NVIDIA end user license agreement (EULA) associated
* with this source code for terms and conditions that govern your use of
* this software. Any use, reproduction, disclosure, or distribution of
* this software and related documentation outside the terms of the EULA
* is strictly prohibited.
*
*/

/**
*  This sample application illustrates the encoding and streaming of a video
*  with one thread while another thread receives and decodes the video.
*  HDR video streaming is also demonstrated in this application.
*/

#include <cuda.h>
#include <iostream>
#include <iomanip>
#include <exception>
#include <stdexcept>
#include <memory>
#include <functional>
#include <stdint.h>
#include "NvDecoder/NvDecoder.h"
#include "NvEncoder/NvEncoderCuda.h"
#include "../Utils/NvEncoderCLIOptions.h"
#include "../Utils/NvCodecUtils.h"
#include "../Utils/FFmpegStreamer.h"
#include "../Utils/FFmpegDemuxer.h"
#include "../Utils/ColorSpace.h"
#include "../Ringmaster/Utils/timestamp.hh"

simplelogger::Logger *logger = simplelogger::LoggerFactory::CreateConsoleLogger();

void ShowHelpAndExit(const char *szBadOption = NULL)
{
    bool bThrowError = false;
    std::ostringstream oss;
    if (szBadOption) 
    {
        bThrowError = true;
        oss << "Error parsing \"" << szBadOption << "\"" << std::endl;
    }
    oss << "Options:" << std::endl
        << "-i           Input file path" << std::endl
        << "-o           Output file path" << std::endl
        << "-s           Input resolution in this form: WxH" << std::endl
        << "-if          Input format: iyuv nv12 p010 bgra bgra64" << std::endl
        << "-of          Output format: native(nv12/p010) bgra bgra64" << std::endl
        << "-gpu         Ordinal of GPU to use" << std::endl
        ;
    oss << NvEncoderInitParam().GetHelpMessage(false, false, true);
    if (bThrowError)
    {
        throw std::invalid_argument(oss.str());
    }
    else
    {
        std::cout << oss.str();
        exit(0);
    }
}

enum OutputFormat
{
    native = 0, bgra, bgra64
};

std::vector<std::string> vstrOutputFormatName = 
{
    "native", "bgra", "bgra64"
};

void ParseCommandLine(int argc, char *argv[], char *szInputFileName, int &nWidth, int &nHeight,
    NV_ENC_BUFFER_FORMAT &eInputFormat, OutputFormat &eOutputFormat, char *szOutputFileName,
    NvEncoderInitParam &initParam, int &iGpu)
{
    std::ostringstream oss;
    int i;
    for (i = 1; i < argc; i++) {
        if (!_stricmp(argv[i], "-h")) {
            ShowHelpAndExit();
        }
        if (!_stricmp(argv[i], "-i")) {
            if (++i == argc) {
                ShowHelpAndExit("-i");
            }
            sprintf(szInputFileName, "%s", argv[i]);
            continue;
        }
        if (!_stricmp(argv[i], "-o")) {
            if (++i == argc) { 
                ShowHelpAndExit("-o");
            }
            sprintf(szOutputFileName, "%s", argv[i]);
            continue;
        }
        if (!_stricmp(argv[i], "-s")) {
            if (++i == argc || 2 != sscanf(argv[i], "%dx%d", &nWidth, &nHeight)) {
                ShowHelpAndExit("-s");
            }
            continue;
        }
        std::vector<std::string> vszFileFormatName = {
            "iyuv", "nv12", "p010", "bgra", "bgra64"
        };
        NV_ENC_BUFFER_FORMAT aFormat[] = {
            NV_ENC_BUFFER_FORMAT_IYUV,
            NV_ENC_BUFFER_FORMAT_NV12,
            NV_ENC_BUFFER_FORMAT_YUV420_10BIT,
            NV_ENC_BUFFER_FORMAT_ARGB,
            NV_ENC_BUFFER_FORMAT_UNDEFINED,
        };
        if (!_stricmp(argv[i], "-if")) {
            if (++i == argc) {
                ShowHelpAndExit("-if");
            }
            auto it = std::find(vszFileFormatName.begin(), vszFileFormatName.end(), argv[i]);
            if (it == vszFileFormatName.end()) {
                ShowHelpAndExit("-if");
            }
            eInputFormat = aFormat[it - vszFileFormatName.begin()];
            continue;
        }
        if (!_stricmp(argv[i], "-of")) {
            if (++i == argc) {
                ShowHelpAndExit("-of");
            }
            auto it = find(vstrOutputFormatName.begin(), vstrOutputFormatName.end(), argv[i]);
            if (it == vstrOutputFormatName.end()) {
                ShowHelpAndExit("-of");
            }
            eOutputFormat = (OutputFormat)(it - vstrOutputFormatName.begin());
            continue;
        }
        if (!_stricmp(argv[i], "-gpu")) {
            if (++i == argc) {
                ShowHelpAndExit("-gpu");
            }
            iGpu = atoi(argv[i]);
            continue;
        }
        // Regard as encoder parameter
        if (argv[i][0] != '-') {
            ShowHelpAndExit(argv[i]);
        }
        oss << argv[i] << " ";
        while (i + 1 < argc && argv[i + 1][0] != '-') {
            oss << argv[++i] << " ";
        }
    }

    // Set VUI parameters for HDR
    std::function<void(NV_ENC_INITIALIZE_PARAMS *pParams)> funcInit = [](NV_ENC_INITIALIZE_PARAMS *pParam)
    {
        if (pParam->encodeGUID == NV_ENC_CODEC_HEVC_GUID)
        {
            NV_ENC_CONFIG_HEVC_VUI_PARAMETERS &hevcVUIParameters = pParam->encodeConfig->encodeCodecConfig.hevcConfig.hevcVUIParameters;
            hevcVUIParameters.videoSignalTypePresentFlag = 1;
            hevcVUIParameters.colourDescriptionPresentFlag = 1;
            hevcVUIParameters.colourMatrix = NV_ENC_VUI_MATRIX_COEFFS_FCC;
        }
        else if (pParam->encodeGUID == NV_ENC_CODEC_H264_GUID)
        {
            NV_ENC_CONFIG_H264_VUI_PARAMETERS &h264VUIParameters = pParam->encodeConfig->encodeCodecConfig.h264Config.h264VUIParameters;
            h264VUIParameters.videoSignalTypePresentFlag = 1;
            h264VUIParameters.colourDescriptionPresentFlag = 1;
            h264VUIParameters.colourMatrix = NV_ENC_VUI_MATRIX_COEFFS_FCC;
        }
		else
		{
            
        }
    };
    initParam = NvEncoderInitParam(oss.str().c_str(), (eInputFormat == NV_ENC_BUFFER_FORMAT_UNDEFINED) ? &funcInit : NULL);
}


void EncodeProc(CUdevice cuDevice, int nWidth, int nHeight, NV_ENC_BUFFER_FORMAT eFormat, NvEncoderInitParam *pEncodeCLIOptions,
    bool bBgra64, const char *szInFilePath, const char *szMediaPath, std::exception_ptr &encExceptionPtr) 
{
    CUdeviceptr dpFrame = 0, dpBgraFrame = 0;
    CUcontext cuContext = NULL;

    ///////////////////////
    uint64_t total_enc_time;
    ///////////////////////

    try
    {
        ck(cuCtxCreate(&cuContext, 0, cuDevice));  // check the device
        NvEncoderCuda enc(cuContext, nWidth, nHeight, eFormat, 3, false, false, false);  // create encoder
        NV_ENC_INITIALIZE_PARAMS initializeParams = { NV_ENC_INITIALIZE_PARAMS_VER };  // init params
        NV_ENC_CONFIG encodeConfig = { NV_ENC_CONFIG_VER };  // enc config
        initializeParams.encodeConfig = &encodeConfig;  // put config into init params

        // Create default enc params using initialized params and CLI options 
        enc.CreateDefaultEncoderParams(&initializeParams, pEncodeCLIOptions->GetEncodeGUID(), 
            pEncodeCLIOptions->GetPresetGUID(), pEncodeCLIOptions->GetTuningInfo());
        pEncodeCLIOptions->SetInitParams(&initializeParams, eFormat);

        // Create encoder using initialized params
        enc.CreateEncoder(&initializeParams);

        // Open input file
        std::ifstream fpIn(szInFilePath, std::ifstream::in | std::ifstream::binary);
        if (!fpIn)
        {
            std::cout << "Unable to open input file: " << szInFilePath << std::endl;
            return;
        }

        // Allocate containers for input frame
        int nHostFrameSize = bBgra64 ? nWidth * nHeight * 8 : enc.GetFrameSize();  // get frame size
        std::unique_ptr<uint8_t[]> pHostFrame(new uint8_t[nHostFrameSize]);  // create host frame
        CUdeviceptr dpBgraFrame = 0;  // create pointer to device frame
        ck(cuMemAlloc(&dpBgraFrame, nWidth * nHeight * 8));
        int nFrame = 0;  // frame counter

        // Create streamer
        std::streamsize nRead = 0;
        FFmpegStreamer streamer(pEncodeCLIOptions->IsCodecH264() ? AV_CODEC_ID_H264 : pEncodeCLIOptions->IsCodecHEVC() ? AV_CODEC_ID_HEVC : AV_CODEC_ID_AV1, nWidth, nHeight, 25, szMediaPath);
        
        do {

            std::vector<std::vector<uint8_t>> vPacket; 
            nRead = fpIn.read(reinterpret_cast<char*>(pHostFrame.get()), nHostFrameSize).gcount(); 
            if (nRead == nHostFrameSize) // if not end of file
            {
                // Get the next available input buffer (device memory)
                const NvEncInputFrame* encoderInputFrame = enc.GetNextInputFrame();

                if (bBgra64)
                {
                    ck(cuMemcpyHtoD(dpBgraFrame, pHostFrame.get(), nHostFrameSize));
                    Bgra64ToP016((uint8_t *)dpBgraFrame, nWidth * 8, (uint8_t *)encoderInputFrame->inputPtr, encoderInputFrame->pitch, nWidth, nHeight);
                }
                else  // copy input data from host memory (pHostFrame.get()) to device memory (encoderInputFrame->inputPtr)
                {   
                    NvEncoderCuda::CopyToDeviceFrame(cuContext, pHostFrame.get(), 0, (CUdeviceptr)encoderInputFrame->inputPtr,
                        (int)encoderInputFrame->pitch,
                        enc.GetEncodeWidth(),
                        enc.GetEncodeHeight(),
                        CU_MEMORYTYPE_HOST,
                        encoderInputFrame->bufferFormat,
                        encoderInputFrame->chromaOffsets,
                        encoderInputFrame->numChromaPlanes);
                }
                
                // Encode the uncompressed data, which has been allocated to dencoderInputFrame
                enc.EncodeFrame(vPacket);

            }
            else
            {
                enc.EndEncode(vPacket);
            }


            // for each packet
            std::cout << "nFrame: " << nFrame << std::endl;
            std::cout << "vPacket size: " << vPacket.size() << std::endl;
            if (vPacket.size() > 0) {
                std::cout << "vPacket[0] size: " << vPacket[0].size() << std::endl;
            }
            for (std::vector<uint8_t> &packet : vPacket) {
                streamer.Stream(packet.data(), (int)packet.size(), nFrame++);
            }
        } while (nRead == nHostFrameSize);

        // Free allocated memory
        ck(cuMemFree(dpBgraFrame));
        dpBgraFrame = 0;

        // Destroy encoder
        enc.DestroyEncoder();
        fpIn.close();

        std::cout << std::flush << "Total frames encoded: " << nFrame << std::endl 
        << "Total encode time: " << total_enc_time << std::endl
        << std::flush;
    }
    catch (const std::exception& )
    {
        encExceptionPtr = std::current_exception();
        ck(cuMemFree(dpBgraFrame));
        dpBgraFrame = 0;
        ck(cuMemFree(dpFrame));
        dpFrame = 0;
    }
}

void DecodeProc(CUdevice cuDevice, const char *szMediaUri, OutputFormat eOutputFormat, const char *szOutFilePath, std::exception_ptr &decExceptionPtr)
{
    CUdeviceptr dpRgbFrame = 0;
    try
    {
        // Create CUDA context
        CUcontext cuContext = NULL;
        ck(cuCtxCreate(&cuContext, 0, cuDevice));
        // Create FFmpeg demuxer
        FFmpegDemuxer demuxer(szMediaUri);
        // Create decoder
        NvDecoder dec(cuContext, eOutputFormat != native, FFmpeg2NvCodecId(demuxer.GetVideoCodec()), true);

        uint8_t *pVideo = NULL;  // pointer to the data buffer that is to be decoded
        int nVideoBytes = 0;  // size of the data buffer in bytes
        int nFrame = 0;  // frame counter

        // Open output file
        std::ofstream fpOut(szOutFilePath, std::ios::out | std::ios::binary);
        if (!fpOut)
        {
            std::ostringstream err;
            err << "Unable to open output file: " << szOutFilePath << std::endl;
            throw std::invalid_argument(err.str());
        }

        // Figure out the bytes per pixel and allocate memory for the RGB frame
        const char *szTail = "\xe0\x00\x00\x00\x01\xce\x8c\x4d\x9d\x10\x8e\x25\xe9\xfe";
        int nWidth = demuxer.GetWidth(), nHeight = demuxer.GetHeight();
        std::unique_ptr<uint8_t[]> pRgbFrame;  // pointer to the RGB frame (host memory)
        int nRgbFramePitch = 0, nRgbFrameSize = 0;
        if (eOutputFormat != native) {
            nRgbFramePitch = nWidth * (eOutputFormat == bgra ? 4 : 8);
            nRgbFrameSize = nRgbFramePitch * nHeight;
            pRgbFrame.reset(new uint8_t[nRgbFrameSize]);
            ck(cuMemAlloc(&dpRgbFrame, nRgbFrameSize));
        }

        do {
            // demux next chunk of data (loop until nVideoBytes is zero)
            demuxer.Demux(&pVideo, &nVideoBytes);  
            uint8_t *pFrame;
            int nFrameReturned = 0;

            // Decode the chunk into NV12/P010/BGRA frames and get nFrameReturned
            if ((demuxer.GetVideoCodec() == AV_CODEC_ID_H264) || (demuxer.GetVideoCodec() == AV_CODEC_ID_HEVC))
            {
                nFrameReturned = dec.Decode(nVideoBytes > 0 ? pVideo + 6 : NULL,
                    // Cut head and tail generated by FFmpegDemuxer for H264/HEVC
                    nVideoBytes - (nVideoBytes > 20 && !memcmp(pVideo + nVideoBytes - 14, szTail, 14) ? 20 : 6),
                    CUVID_PKT_ENDOFPICTURE);
            }
            else
            {
                nFrameReturned = dec.Decode(pVideo, nVideoBytes);
            }

            // get information about the video stream (codec, display parameters etc)
            int iMatrix = dec.GetVideoFormatInfo().video_signal_description.matrix_coefficients;
            if (!nFrame && nFrameReturned) {
                LOG(INFO) << "Color matrix coefficient: " << iMatrix;
            }

            // Fetch all the frames that are available for display
            for (int i = 0; i < nFrameReturned; i++) {
                pFrame = dec.GetFrame();  // returns a decoded frame (and timestamp if the passed pointer is not NULL)

                // Write the frame into the output file
                if (eOutputFormat == native) {
                    fpOut.write(reinterpret_cast<char*>(pFrame), dec.GetFrameSize());
                }
                else {
                    // Color space conversion
                    if (dec.GetBitDepth() == 8) {
                        if (eOutputFormat == bgra) {
                            Nv12ToColor32<BGRA32>(pFrame, nWidth, (uint8_t *)dpRgbFrame, nRgbFramePitch, nWidth, nHeight, iMatrix);
                        }
                        else {
                            Nv12ToColor64<BGRA64>(pFrame, nWidth, (uint8_t *)dpRgbFrame, nRgbFramePitch, nWidth, nHeight, iMatrix);
                        }
                    }
                    else {
                        if (eOutputFormat == bgra) {
                            P016ToColor32<BGRA32>(pFrame, nWidth * 2, (uint8_t *)dpRgbFrame, nRgbFramePitch, nWidth, nHeight, iMatrix);
                        }
                        else {
                            P016ToColor64<BGRA64>(pFrame, nWidth * 2, (uint8_t *)dpRgbFrame, nRgbFramePitch, nWidth, nHeight, iMatrix);
                        }
                    }

                    // // print out the size of pRgbFrame
                    // std::cout << "pRgbFrame size: " << nRgbFrameSize << std::endl;
                    // std::cout << "nRGBFramePitch: " << nRgbFramePitch << std::endl;
                    // std::cout << "display_width_: " << nWidth << std::endl;
                    // std::cout << "display_height_: " << nHeight << std::endl;
                    // std::cout << "pRgbFrame.get() size: " << sizeof(pRgbFrame.get()) << std::endl;
                    // std::cout << "dec.getframesize():" << dec.GetFrameSize() << std::endl;

                    ck(cuMemcpyDtoH(pRgbFrame.get(), dpRgbFrame, nRgbFrameSize));
                    fpOut.write(reinterpret_cast<char*>(pRgbFrame.get()), nRgbFrameSize);
                }
                nFrame++;
            }

        } while (nVideoBytes);

        // Clean up
        if (eOutputFormat != native) 
        {
            ck(cuMemFree(dpRgbFrame));
            dpRgbFrame = 0;
            pRgbFrame.reset(nullptr);
        }
        fpOut.close();

        std::cout << "Total frame decoded: " << nFrame << std::endl
            << "Saved in file " << szOutFilePath << " in "
            << (eOutputFormat == native ? (dec.GetBitDepth() == 8 ? "nv12" : "p010") : (eOutputFormat == bgra ? "bgra" : "bgra64"))
            << " format" << std::endl;
    }
    catch (const std::exception &)
    {
        decExceptionPtr = std::current_exception();
        cuMemFree(dpRgbFrame);
    }
}

int main(int argc, char **argv)
{
    // initialize variables
    char szInFilePath[256] = "",
        szOutFilePath[256] = "";
    int nWidth = 0, nHeight = 0;
    NV_ENC_BUFFER_FORMAT eInputFormat = NV_ENC_BUFFER_FORMAT_IYUV;
    OutputFormat eOutputFormat = native;
    int iGpu = 0;
    bool bBgra64 = false;
    std::exception_ptr encExceptionPtr;
    std::exception_ptr decExceptionPtr;

    try
    {
        // Parse command line parameters
        NvEncoderInitParam encodeCLIOptions;
        ParseCommandLine(argc, argv, szInFilePath, nWidth, nHeight, eInputFormat, eOutputFormat, szOutFilePath, encodeCLIOptions, iGpu);

        // Sanity checks
        CheckInputFile(szInFilePath);
        ValidateResolution(nWidth, nHeight);

        // Check input format
        if (eInputFormat == NV_ENC_BUFFER_FORMAT_UNDEFINED) {
            bBgra64 = true;
            eInputFormat = NV_ENC_BUFFER_FORMAT_YUV420_10BIT;
        }

        // Check output format
        if (!*szOutFilePath) {
            sprintf(szOutFilePath, "out.%s", eOutputFormat != native ?
                vstrOutputFormatName[eOutputFormat].c_str() :
                (eInputFormat != NV_ENC_BUFFER_FORMAT_YUV420_10BIT ? "nv12" : "p010"));
        }

        // Check cuda device
        ck(cuInit(0));
        int nGpu = 0;
        ck(cuDeviceGetCount(&nGpu));
        if (iGpu < 0 || iGpu >= nGpu) {
            std::cout << "GPU ordinal out of range. Should be within [" << 0 << ", " << nGpu - 1 << "]" << std::endl;
            return 1;
        }
        CUdevice cuDevice = 0;
        ck(cuDeviceGet(&cuDevice, iGpu));
        char szDeviceName[80];
        ck(cuDeviceGetName(szDeviceName, sizeof(szDeviceName), cuDevice));
        std::cout << "GPU in use: " << szDeviceName << std::endl;

        // Launch threads
        const char *szMediaUri = "tcp://127.0.0.1:8899";
        char szMediaUriDecode[1024];
        sprintf(szMediaUriDecode, "%s?listen", szMediaUri);
        NvThread thDecode(std::thread(DecodeProc, cuDevice, szMediaUriDecode, eOutputFormat, szOutFilePath, std::ref(decExceptionPtr)));
        NvThread thEncode(std::thread(EncodeProc, cuDevice, nWidth, nHeight, eInputFormat, &encodeCLIOptions, bBgra64, szInFilePath, szMediaUri, std::ref(encExceptionPtr)));
        thEncode.join();
        thDecode.join();
        if (encExceptionPtr)
        {
            std::rethrow_exception(encExceptionPtr);
        }
        if (decExceptionPtr)
        {
            std::rethrow_exception(decExceptionPtr);
        }
    }
    catch (const std::exception &ex)
    {
        std::cout << ex.what();
        exit(1);
    }
    return 0;
}
