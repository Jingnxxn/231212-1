# 231212-1
---
- MainWindow.xaml.cpp
```ruby
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

```
- MainWindow.xaml
```ruby
<!-- Copyright (c) Microsoft Corporation and Contributors. -->
<!-- Licensed under the MIT License. -->

<Window
    x:Class="App12.MainWindow"
    xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
    xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
    xmlns:local="using:App12"
    xmlns:d="http://schemas.microsoft.com/expression/blend/2008"
    xmlns:mc="http://schemas.openxmlformats.org/markup-compatibility/2006"
    xmlns:canvas="using:Microsoft.Graphics.Canvas.UI.Xaml"
    mc:Ignorable="d">

    <Grid>
        <Grid.RowDefinitions>
            <RowDefinition Height="60"/>
            <RowDefinition Height="590"/>
        </Grid.RowDefinitions>
        <Grid.ColumnDefinitions>
            <ColumnDefinition Width="600"/>
            <ColumnDefinition Width="400"/>
        </Grid.ColumnDefinitions>
    

        <StackPanel Orientation="Horizontal" HorizontalAlignment="Center" VerticalAlignment="Center">
            <Slider AutomationProperties.Name="simple slider" Width="200"
                    Grid.Column="0" Grid.Row="0" x:Name="SliderControl"
                    ValueChanged="Slider_ValueChanged"/>
            <Button x:Name="myWrite" Click="myWrite_Click">Write</Button>
            <Button x:Name="myRead" Click="myRead_Click">Read</Button>
            <Button x:Name="myClear" Click="myClear_Click">Clear</Button>
        </StackPanel>
        <AppBarToggleButton Grid.Column="1" Grid.Row="0"
                            x:Name="ColorPicker" Icon="Shuffle" IsChecked="True"
                            Label="Enable" Click="ColorPicker_Click"/>

        <canvas:CanvasControl Grid.Column="0" Grid.Row="1"
            PointerPressed="CanvasControl_PointerPressed"
            PointerReleased="CanvasControl_PointerReleased"
            PointerMoved="CanvasControl_PointerMoved"
            Draw="CanvasControl_Draw" ClearColor="CornflowerBlue"/>

        <Border Grid.Column="1" Grid.Row="1" x:Name="colorPanel" Visibility="Visible">
            <ColorPicker Grid.Column="1" Grid.Row="1" 
            ColorChanged="ColorPicker_ColorChanged"
            ColorSpectrumShape="Ring"
            IsMoreButtonVisible="False"
            IsColorSliderVisible="True"
            IsColorChannelTextInputVisible="True"
            IsHexInputVisible="True"
            IsAlphaEnabled="False"
            IsAlphaSliderVisible="True"
            IsAlphaTextInputVisible="True" />
            
        </Border>
    </Grid>
</Window>
```
- MainWindow.xaml.h
```ruby
// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "MainWindow.g.h"
#include <winrt/Microsoft.Graphics.Canvas.UI.Xaml.h>
#include <winrt/Microsoft.UI.Input.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
using namespace winrt::Microsoft::UI;
using namespace winrt::Microsoft::Graphics::Canvas::UI::Xaml;

namespace winrt::App12::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow();

        int32_t MyProperty();
        void MyProperty(int32_t value);
        bool flag;
        bool IsChecked;
        float px, py;
        float mySize;
        std::vector<float> vx;
        std::vector<float> vy;
        std::vector<struct winrt::Windows::UI::Color> col;
        std::vector<float> size;

        void Slider_ValueChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs const& e);
        void ColorPicker_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void ColorPicker_ColorChanged(winrt::Microsoft::UI::Xaml::Controls::ColorPicker const& sender, winrt::Microsoft::UI::Xaml::Controls::ColorChangedEventArgs const& args);
        void CanvasControl_PointerPressed(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e);
        void CanvasControl_PointerReleased(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e);
        void CanvasControl_PointerMoved(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e);
        void CanvasControl_Draw(winrt::Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl const& sender, winrt::Microsoft::Graphics::Canvas::UI::Xaml::CanvasDrawEventArgs const& args);
        void myWrite_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void myRead_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void myClear_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::App12::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
```
-> 실행화면 :

![image](https://github.com/Jingnxxn/VisualProgramming/assets/96435960/4b276461-0966-463d-986d-91049b360860)

![image](https://github.com/Jingnxxn/VisualProgramming/assets/96435960/d9a6b9ed-ff09-4b16-9d57-3e47236031d5)


![image](https://github.com/Jingnxxn/VisualProgramming/assets/96435960/1e0473d2-f39c-4765-aba7-51fc3697a346)
