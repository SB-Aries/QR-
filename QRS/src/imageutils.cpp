#include "imageutils.h"
#include <string>
#include <vector>
#include <Windows.h>
#include <wincodec.h>

using namespace std;

void ImageUtils::saveQRAsPNG(const vector<bool>& qrMatrix, int size, const string& outputPath) {
    // 使用Windows WIC API保存PNG文件
    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr)) {
        return;
    }
    
    IWICImagingFactory* pFactory = nullptr;
    hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFactory));
    if (FAILED(hr)) {
        CoUninitialize();
        return;
    }
    
    // 计算最终图像大小：QR码大小 + 4模块静区 * 2，每个模块8x8像素
    int moduleSize = 8;
    int quietZone = 4;
    int imageSize = (size + 2 * quietZone) * moduleSize;
    
    // 创建位图
    IWICBitmap* pBitmap = nullptr;
    hr = pFactory->CreateBitmap(imageSize, imageSize, GUID_WICPixelFormat8bppGray, WICBitmapCacheOnDemand, &pBitmap);
    if (FAILED(hr)) {
        pFactory->Release();
        CoUninitialize();
        return;
    }
    
    // 锁定位图进行写入
    WICRect rect = {0, 0, imageSize, imageSize};
    UINT stride = 0;
    BYTE* pPixels = nullptr;
    hr = pBitmap->Lock(&rect, WICBitmapLockReadWrite, &stride, &pPixels);
    if (FAILED(hr)) {
        pBitmap->Release();
        pFactory->Release();
        CoUninitialize();
        return;
    }
    
    // 填充背景为白色（255）
    for (int y = 0; y < imageSize; ++y) {
        for (int x = 0; x < imageSize; ++x) {
            pPixels[y * stride + x] = 255;
        }
    }
    
    // 绘制QR码
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            bool isBlack = qrMatrix[y * size + x];
            if (isBlack) {
                // 绘制8x8像素的模块
                for (int dy = 0; dy < moduleSize; ++dy) {
                    for (int dx = 0; dx < moduleSize; ++dx) {
                        int px = (x + quietZone) * moduleSize + dx;
                        int py = (y + quietZone) * moduleSize + dy;
                        pPixels[py * stride + px] = 0;
                    }
                }
            }
        }
    }
    
    // 解锁位图
    IWICBitmapLock* pLock = nullptr;
    hr = pBitmap->Unlock();
    if (FAILED(hr)) {
        pBitmap->Release();
        pFactory->Release();
        CoUninitialize();
        return;
    }
    
    // 创建PNG编码器
    IWICBitmapEncoder* pEncoder = nullptr;
    hr = pFactory->CreateEncoder(GUID_ContainerFormatPng, nullptr, &pEncoder);
    if (FAILED(hr)) {
        pBitmap->Release();
        pFactory->Release();
        CoUninitialize();
        return;
    }
    
    // 创建文件流
    IWICStream* pStream = nullptr;
    hr = pFactory->CreateStream(&pStream);
    if (FAILED(hr)) {
        pEncoder->Release();
        pBitmap->Release();
        pFactory->Release();
        CoUninitialize();
        return;
    }
    
    // 打开文件
    hr = pStream->InitializeFromFilename(outputPath.c_str(), GENERIC_WRITE);
    if (FAILED(hr)) {
        pStream->Release();
        pEncoder->Release();
        pBitmap->Release();
        pFactory->Release();
        CoUninitialize();
        return;
    }
    
    // 初始化编码器
    hr = pEncoder->Initialize(pStream, WICBitmapEncoderNoCache);
    if (FAILED(hr)) {
        pStream->Release();
        pEncoder->Release();
        pBitmap->Release();
        pFactory->Release();
        CoUninitialize();
        return;
    }
    
    // 创建帧
    IWICBitmapFrameEncode* pFrame = nullptr;
    hr = pEncoder->CreateNewFrame(&pFrame, nullptr);
    if (FAILED(hr)) {
        pStream->Release();
        pEncoder->Release();
        pBitmap->Release();
        pFactory->Release();
        CoUninitialize();
        return;
    }
    
    // 初始化帧
    hr = pFrame->Initialize(nullptr);
    if (FAILED(hr)) {
        pFrame->Release();
        pStream->Release();
        pEncoder->Release();
        pBitmap->Release();
        pFactory->Release();
        CoUninitialize();
        return;
    }
    
    // 写入位图
    hr = pFrame->WriteSource(pBitmap, nullptr);
    if (FAILED(hr)) {
        pFrame->Release();
        pStream->Release();
        pEncoder->Release();
        pBitmap->Release();
        pFactory->Release();
        CoUninitialize();
        return;
    }
    
    // 提交帧
    hr = pFrame->Commit();
    if (FAILED(hr)) {
        pFrame->Release();
        pStream->Release();
        pEncoder->Release();
        pBitmap->Release();
        pFactory->Release();
        CoUninitialize();
        return;
    }
    
    // 提交编码器
    hr = pEncoder->Commit();
    
    // 释放资源
    pFrame->Release();
    pStream->Release();
    pEncoder->Release();
    pBitmap->Release();
    pFactory->Release();
    CoUninitialize();
}

vector<bool> ImageUtils::loadPNGAndDecode(const string& filePath) {
    // 使用Windows WIC API加载PNG文件并解码
    // 这里需要实现具体的PNG加载和解码逻辑
    // 暂时返回空数据
    return vector<bool>();
}