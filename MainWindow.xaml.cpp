// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#include <fstream>
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::App12::implementation
{
    MainWindow::MainWindow()
    {
        InitializeComponent();
        flag = false;
        px = 100;
        py = 100;
        mySize = 16;
        IsChecked = true;
    }

    int32_t MainWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    
}

struct winrt::Windows::UI::Color myCol = winrt::Microsoft::UI::Colors::Green();


void winrt::App12::implementation::MainWindow::Slider_ValueChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs const& e)
{
    mySize = e.NewValue();

    
}


void winrt::App12::implementation::MainWindow::ColorPicker_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    if (IsChecked) {
        IsChecked = FALSE;
        ColorPicker().Label(L"Disable");
        colorPanel().Visibility(Visibility::Collapsed);
        SliderControl().Visibility(Visibility::Collapsed);
    }
    else {
        IsChecked = TRUE;
        ColorPicker().Label(L"Enable");
        colorPanel().Visibility(Visibility::Visible);
        SliderControl().Visibility(Visibility::Visible);
    }
}


void winrt::App12::implementation::MainWindow::ColorPicker_ColorChanged(winrt::Microsoft::UI::Xaml::Controls::ColorPicker const& sender, winrt::Microsoft::UI::Xaml::Controls::ColorChangedEventArgs const& args)
{
    myCol = args.NewColor();
}


void winrt::App12::implementation::MainWindow::CanvasControl_PointerPressed(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e)
{
    flag = true;
}


void winrt::App12::implementation::MainWindow::CanvasControl_PointerReleased(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e)
{
    flag = false;
    px = py = 0.0;
    vx.push_back(px);
    vy.push_back(py);
    col.push_back(myCol);
    size.push_back(mySize);
}


void winrt::App12::implementation::MainWindow::CanvasControl_PointerMoved(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e)
{
    CanvasControl canvas = sender.as<CanvasControl>();
    px = e.GetCurrentPoint(canvas).Position().X;
    py = e.GetCurrentPoint(canvas).Position().Y;
    if (flag) {
        vx.push_back(px);
        vy.push_back(py);
        col.push_back(myCol);
        size.push_back(mySize);
        canvas.Invalidate();
    }
}


void winrt::App12::implementation::MainWindow::CanvasControl_Draw(winrt::Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl const& sender, winrt::Microsoft::Graphics::Canvas::UI::Xaml::CanvasDrawEventArgs const& args)
{
    CanvasControl canvas = sender.as<CanvasControl>();
    int n = vx.size();
    for (int i = 1; i < n; i++) {
        if (vx[i] == 0 && vy[i] == 0) {
            i++; continue;
        }

        args.DrawingSession().DrawLine(vx[i - 1], vy[i - 1], vx[i], vy[i], col[i], size[i]);
        args.DrawingSession().FillCircle(vx[i - 1], vy[i - 1], size[i] / 2, col[i]);
        args.DrawingSession().FillCircle(vx[i], vy[i], size[i] / 2, col[i]);
    }
    canvas.Invalidate();
}


void winrt::App12::implementation::MainWindow::myWrite_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    std::ofstream outFile("C:/Users/user/source/repos/App12/my.txt");

    if (outFile.is_open())
    {
        int num = vx.size();
        outFile << num << "\n";

        for (int i = 0; i < num; i++) {
            outFile << vx[i] << " " << vy[i] << " "
                << static_cast<int>(col[i].A) << " " << static_cast<int>(col[i].B) << " "
                << static_cast<int>(col[i].G) << " " << static_cast<int>(col[i].R) << " "
                << size[i] << "\n";
        }

        outFile.close();
        MessageBox(NULL, L"파일이 저장되었습니다\n", L"성공", NULL);
    }
    else {
        MessageBox(NULL, L"파일이 저장되지 않았습니다\n", L"오류", NULL);
    }

}


void winrt::App12::implementation::MainWindow::myRead_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    std::ifstream inFile("C:/Users/user/source/repos/App12/my.txt");

    if (inFile.is_open())
    {
        vx.clear();
        vy.clear();
        col.clear();
        size.clear();

        int num;
        float vx1, vy1, size1;
        int colA, colB, colG, colR;

        inFile >> num;

        for (int i = 0; i < num; i++) {
            inFile >> vx1 >> vy1 >> colA >> colB >> colG >> colR >> size1;

            vx.push_back(vx1);
            vy.push_back(vy1);

            myCol.A = static_cast<uint8_t>(colA);
            myCol.B = static_cast<uint8_t>(colB);
            myCol.G = static_cast<uint8_t>(colG);
            myCol.R = static_cast<uint8_t>(colR);

            col.push_back(myCol);
            size.push_back(size1);
        }

        inFile.close();
        MessageBox(NULL, L"파일이 로드되었습니다\n", L"성공", NULL);

        CanvasControl_PointerReleased(NULL, NULL);
    }
    else {
        MessageBox(NULL, L"파일이 로드되지 않았습니다\n", L"오류", NULL);
    }

}


void winrt::App12::implementation::MainWindow::myClear_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    vx.clear();
    vy.clear();
    col.clear();
    size.clear();
    flag = false;
    px = 100;
    py = 100;
    mySize = 16;
}

