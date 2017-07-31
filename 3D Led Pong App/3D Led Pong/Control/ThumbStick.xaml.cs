using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using Windows.Storage.Streams;
using System.Diagnostics;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

namespace Led_Pong_App.Control
{
    public sealed partial class ThumbStick : UserControl
    {
        public event EventHandler<string> NewPosition;
        private int _snapValue = 10;
        public int xVal,yVal, old_xVal, old_yVal;


        public ThumbStick()
        {
            InitializeComponent();
            textBlockXVal.Text = App.dataServiceDevice.Name;

            //this.DataContext = this;

            //this.Resources.Add("yVal", xVal);
        }
        private void LayoutRoot_OnMouseMove(object sender, PointerRoutedEventArgs e)
        {
            WorkOutPosition(new Point(e.GetCurrentPoint(LayoutRoot).Position.X, e.GetCurrentPoint(LayoutRoot).Position.Y));

            //xVal = 5;
        }
        private void LayoutRoot_OnMouseLeftButtonDown(object sender, PointerRoutedEventArgs e)
        {
            //Capture the the mouse with the grid so it receives all finger events

            //((Grid)sender).CapturePointer(e.Pointer);
            LayoutRoot.CapturePointer(e.Pointer);
            //e.GetPosition(LayoutRoot);
            WorkOutPosition(new Point(e.GetCurrentPoint(LayoutRoot).Position.X, e.GetCurrentPoint(LayoutRoot).Position.Y));
        }

        private void LayoutRoot_OnMouseLeftButtonUp(object sender, PointerRoutedEventArgs e)
        {
            LayoutRoot.ReleasePointerCapture(e.Pointer);
            //return pos to middle
            //WorkOutPosition(new Point(150, 150));
        }

        private async void WorkOutPosition(Point newPos)
        {
            //make the postions snap in the middle if they haven't moved more that the snapvalue.
            //also make sure the position isn't outside the square
            newPos.X = Clamp((newPos.X > -_snapValue && newPos.X < _snapValue) ? 0 : newPos.X, 0, 300);
            newPos.Y = Clamp((newPos.Y > -_snapValue && newPos.Y < _snapValue) ? 0 : newPos.Y, 0, 300);

            //move the crosshair. Remember that the crosshair transform is 0,0 in the middle 
            //whereas the "newPos" value is 0,0 at the top left of the box.
            crosshairTransform.TranslateX = newPos.X - 150;
            crosshairTransform.TranslateY = newPos.Y - 150;

            double fxVal = (newPos.X / 300)*7;
            double fyVal = (newPos.Y / 300) * 7;

            xVal = (int)Math.Round(fxVal);
            yVal = (int)Math.Round(fyVal);

            if ( (xVal != old_xVal || yVal != old_yVal ) )
            {
                old_xVal = xVal;
                old_yVal = yVal;
                /*
                textBlockXVal.Text = "X:  " + xVal.ToString();
                textBlockYVal.Text = "Y:  " + yVal.ToString();
                */
                //var n = App._btWriter.WriteString("X:" + xVal.ToString() + " Y:" + yVal.ToString() + "\n");
                var n = App._btWriter.WriteString("c"+xVal.ToString() + yVal.ToString() + "\n");
                await App._btWriter.StoreAsync();
                
            }
            
        }

        private double Clamp(double value, double min, double max)
        {
            return value < min ? min : value > max ? max : value;
        }
    }
}
