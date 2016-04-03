/**********************************************************************************************//**
 * @file    MainWindow.xaml.cs
 *
 * @brief   Implements the main window.xaml class.
 *          This file contains all GUI specific code (Event handler for GUI-elements). 
 **************************************************************************************************/

using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Media3D;
using System.IO.Ports;
using System.Text.RegularExpressions;
using HelixToolkit.Wpf;
using System.Collections.Generic;


namespace LIDAR_Controller
{
    /**********************************************************************************************//**
     * @class   MainWindow
     *
     * @brief   The application's main form.
     *          This class contains all necessary event and exception handlers.
     *
     * @author  Alexander Miller (7089316)
     * @date    22.12.2015
     **************************************************************************************************/

    public partial class MainWindow : Window
    {

        /** @brief   The COM port which is used to communicate with the LIDAR-Scanner. */
        SerialPort ComPort = null;


        /** @brief   The actually selected measurement. */
        int selectedMeasure;


        /** @brief   List of all measurements. */
        List<Measurement> MeasureList = new List<Measurement>();


        /** @brief   A viewport object that handles all 3D-Drawings. */
        HelixViewport3D myViewport = new HelixViewport3D();


        /** @brief   The grid lines of the xy-plane. */
        GridLinesVisual3D gridLinesXY = new GridLinesVisual3D();

        /**********************************************************************************************//**
         * @fn  public MainWindow()
         *
         * @brief   Default constructor of MainWindow.
         *          This constructor does all the initialisation work. At first it initialises all GUI objects. 
         *          Then it calls the init() function. This function adds COM-Port informations to some GUI objects. 
         *          The last function initializes the Viewport object.
         *
         * @author  Alexander Miller (7089316)
         * @date    22.12.2015
         **************************************************************************************************/

        public MainWindow()
        {
            InitializeComponent();
            Init();
            Init3D();
        }

        /**********************************************************************************************//**
         * @fn  private void ComPortReceiveHandler(object Sender, SerialDataReceivedEventArgs e)
         *
         * @brief   Handler, called when the COM-port receives something.
         *          
         *          a: if no problems occur then...   
         *          
         *          1. Check if ComPort is open  
         *          2. Read line from buffer  
         *          3. Add the received line to textBox1  
         *          4. Get relevant data from string using a regular expression  
         *          5. Parse data into integers  
         *          6. Set a point of the selected measurement based on the received data.  
         *          
         *          b: If something goes wrong, call the exception handler.
         *
         * @author  Alexander Miller (7089316)
         * @date    22.12.2015
         *
         * @param   Sender  Source of the event.
         * @param   e       Serial data received event information.
         **************************************************************************************************/

        private void ComPortReceiveHandler(object Sender, SerialDataReceivedEventArgs e)
        {
            //a
            try
            {
                //1
                if (!ComPort.IsOpen) return;
                string ReceivedText = "";
                //2
                ReceivedText = ComPort.ReadLine();
                //3
                Dispatcher.BeginInvoke(new Action(() =>
                {
                    this.textBox1.AppendText(ReceivedText);
                    this.textBox1.ScrollToEnd();
                }));
                //4
                string[] numbers = Regex.Split(ReceivedText, @"\D+");
                //5
                if (numbers.Length == 5)
                {
                    int mpos = 0;
                    int spos = 0;
                    int value = 0;
                    if (int.TryParse(numbers[1], out mpos) && int.TryParse(numbers[2], out spos) && int.TryParse(numbers[3],out value))
                        //6
                        MeasureList[selectedMeasure].setDistanceData(mpos+1, spos, value);
                    Dispatcher.BeginInvoke(new Action(() =>
                    {
                        MeasureList[selectedMeasure].setGeometryPoint3D(mpos+1 , spos);

                    }));



                }
            }
            //b
            catch (Exception ex) { ExeptionHandler(ex); }
        }

        /**********************************************************************************************//**
         * @fn  private void ExeptionHandler(Exception ex)
         *
         * @brief   Handler, called when a exception occurs.
         *
         * @author  Alexander Miller (7089316)
         * @date    22.12.2015
         *
         * @param   ex  The exception that occurred.
         **************************************************************************************************/

        private void ExeptionHandler(Exception ex)
        {
            switch (ex.GetType().Name)
            {
                case "IOException":
                    MessageBox.Show("Name: " + ex.GetType().Name + "\r\nBeschreibung:\r\n" + ex.Message, "Fehler!", MessageBoxButton.OK, MessageBoxImage.Error);
                    break;
                case "ArgumentException":
                    MessageBox.Show("Bitte Eingaben überprüfen!", "Fehleingabe erkannt!", MessageBoxButton.OK, MessageBoxImage.Information);
                    break;
                default:
                    MessageBox.Show("Name: " + ex.GetType().Name + "\r\nBeschreibung:\r\n" + ex.Message, "Unbekannter Fehler!", MessageBoxButton.OK, MessageBoxImage.Error);
                    break;
            }

        }

        /**********************************************************************************************//**
         * @fn  private void Init()
         *
         * @brief   Initialises both combo boxes with relevant data (e.g. available COM-Ports).
         *
         * @author  Alexander Miller (7089316)
         * @date    22.12.2015
         **************************************************************************************************/

        private void Init()
        {
            //add all COM-Ports to combobox
            foreach (string ports in SerialPort.GetPortNames())
            {
                comboBox.Items.Add(ports);
            }
            //initial measurement
            MeasureList.Add(new Measurement(new Vector3D(0, 0, 0.5), 0, 0));

            //add measurement to combobox2 and register a new event handler for "SelectionChangedEvent"
            comboBox2.Items.Add(MeasureList[selectedMeasure]);
            comboBox2.DisplayMemberPath = "mId";
            comboBox2.SelectionChanged += new SelectionChangedEventHandler(comboBox2_SelectionChanged);
            comboBox2.SelectedIndex = 0;
            //Enable/Disable all GUI-Elements that need a connected COM-Port
            enableVisuals(false);

        }

        /**********************************************************************************************//**
         * @fn  void comboBox2_SelectionChanged(object Sender, SelectionChangedEventArgs e)
         *
         * @brief   Event handler. Called by comboBox2 for selection changed events.
         *          This handler gets the offset values of the selected measurement and displays them.
         *
         * @author  Alexander Miller (7089316)
         * @date    22.12.2015
         *
         * @param   Sender  Source of the event.
         * @param   e       Selection changed event information.
         **************************************************************************************************/

        void comboBox2_SelectionChanged(object Sender, SelectionChangedEventArgs e)
        {
            selectedMeasure = comboBox2.SelectedIndex;
            if (selectedMeasure >= 0)
            {
                xOffsetTxt.Text = "" + MeasureList[selectedMeasure].linearOffset.X;
                yOffsetTxt.Text = "" + MeasureList[selectedMeasure].linearOffset.Y;
                zOffsetTxt.Text = "" + MeasureList[selectedMeasure].linearOffset.Z;
                degOffsetXTxt.Text = "" + MeasureList[selectedMeasure].rotaryOffsetX;
                degOffsetZTxt.Text = "" + MeasureList[selectedMeasure].rotaryOffsetZ;
            }

        }

        /**********************************************************************************************//**
         * @fn  private void Init3D()
         *
         * @brief   Initialises the viewport object.
         *          1. Configure the grid lines for the xy-Plane.  
         *          2. Configure myViewport to show additional data.  
         *          3. Add gridLinesXY, a default light and the 3D-Model of the selected measurement to myViewport as children.  
         *          4. Add myViewport to grid (GUI-Element) as child and refresh.
         *
         * @author  Alexander Miller (7089316)
         * @date    22.12.2015
         **************************************************************************************************/

        private void Init3D()
        {
            //1
            gridLinesXY.MinorDistance = 10; //Distance in cm for the minor lines 
            gridLinesXY.MajorDistance = 100; //Distance in cm for the major lines 
            gridLinesXY.Thickness = 1.5; //Line thickness
            gridLinesXY.Length = 10000; //Maximum grid size
            gridLinesXY.Width = 10000; //Maximum grid size

            //2
            myViewport.ShowFrameRate = true; //Show framerate
            myViewport.ShowCoordinateSystem = true; //Show small Coordinate system
            myViewport.ShowFieldOfView = true; //Show Field of View

            //3
            myViewport.Children.Add(gridLinesXY);
            myViewport.Children.Add(new DefaultLights());
            myViewport.Children.Add(MeasureList[selectedMeasure].getGeometry3D());

            //4
            grid.Children.Add(myViewport);
            grid.UpdateLayout();

        }

        /**********************************************************************************************//**
         * @fn  private void verbinden_Click(object sender, RoutedEventArgs e)
         *
         * @brief   Event handler. Called by verbinden for click events.
         *          
         *          a: if no problems occur then...     
         *          
         *             1. If no device is connected then ...    
         *             
         *              1.1. Get baud rate from combobox1  \n
         *              1.2. Create a new connection  \n
         *              1.3. Register a handler for "DataReceived"-Events  \n
         *              1.4. Open connection   \n
         *              1.5. Clear all buffers  \n
         *              1.6. Enable all GUI-Elements   \n
         *                 
         *             2. else     
         *             
         *              2.1. Check if the connection is still open  \n
         *              2.2. Clear all buffers  \n
         *              2.3. Close the connection  \n
         *              2.4. Delete the connection  \n
         *              2.5. Disable some GUI-Elements
         *                  
         *          b: If something goes wrong, call the exception handler.
         *
         * @author  Alexander Miller (7089316)
         * @date    22.12.2015
         *
         * @param   sender  Source of the event.
         * @param   e       Routed event information.
         **************************************************************************************************/

        private void verbinden_Click(object sender, RoutedEventArgs e)
        {
            //a
            try
            {
                //1
                if (ComPort == null)
                {
                    //1.1
                    int baud;
                    Int32.TryParse(comboBox1.Text, out baud);
                    //1.2
                    ComPort = new SerialPort(comboBox.Text, baud);
                    ComPort.DtrEnable = true;
                    //1.3
                    ComPort.DataReceived += ComPortReceiveHandler;
                    //1.4
                    ComPort.Open();
                    //1.5
                    ComPort.DiscardInBuffer();
                    ComPort.DiscardOutBuffer();

                    textBox1.Clear();

                    button.Content = "Trennen";
                    label.Content = "Verbunden";
                    //1.6
                    enableVisuals(true);

                }
                //2
                else
                {
                    //2.1
                    if (ComPort.IsOpen)
                    {
                        //2.2
                        ComPort.DiscardInBuffer();
                        ComPort.DiscardOutBuffer();
                        //2.3
                        ComPort.Close();
                        //2.4
                        ComPort = null;
                        button.Content = "Verbinden";
                        label.Content = "Nicht verbunden!";
                        //2.5
                        enableVisuals(false);
                    }
                }
            }
            //b
            catch (Exception ex)
            {
                ExeptionHandler(ex);
            }
        }

        /**********************************************************************************************//**
         * @fn  private void button2_Click(object sender, RoutedEventArgs e)
         *
         * @brief   Event handler. Called by button2 for click events.
         *          This functions sends the "#1" (Onetime Measure) command to the LIDAR-Scanner.
         *
         * @author  Alexander Miller (7089316)
         * @date    22.12.2015
         *
         * @param   sender  Source of the event.
         * @param   e       Routed event information.
         **************************************************************************************************/

        private void button2_Click(object sender, RoutedEventArgs e)
        {
            ComPort.WriteLine("#1");
        }

        /**********************************************************************************************//**
         * @fn  private void button3_Click(object sender, RoutedEventArgs e)
         *
         * @brief   Event handler. Called by button3 for click events.
         *          This functions sends the "#2" (Radar mode 2D) command to the LIDAR-Scanner.
         *
         * @author  Alexander Miller (7089316)
         * @date    22.12.2015
         *
         * @param   sender  Source of the event.
         * @param   e       Routed event information.
         **************************************************************************************************/

        private void button3_Click(object sender, RoutedEventArgs e)
        {
            ComPort.WriteLine("#2");
        }

        /**********************************************************************************************//**
         * @fn  private void posBtn_Click(object sender, RoutedEventArgs e)
         *
         * @brief   Event handler. Called by posBtn for click events.
         *          This functions sends the "#3" (Set Position) command with the position infos from "txt_MPos" & "txt_SPos" to the LIDAR-Scanner.
         *
         * @author  Alexander Miller (7089316)
         * @date    22.12.2015
         *
         * @param   sender  Source of the event.
         * @param   e       Routed event information.
         **************************************************************************************************/

        private void posBtn_Click(object sender, RoutedEventArgs e)
        {
            int m;
            int s;
            if (int.TryParse(txt_MPos.Text, out m) && (int.TryParse(txt_SPos.Text, out s)))
            {
                ComPort.WriteLine("#3");
                ComPort.WriteLine("" + m);
                ComPort.WriteLine("" + s);
            }
            else
            {
                MessageBox.Show("Es sind nur positive ganze Zahlen als Position erlaubt!", "Fehleingabe erkannt!", MessageBoxButton.OK, MessageBoxImage.Information);
            }

        }

        /**********************************************************************************************//**
         * @fn  private void button6_Click(object sender, RoutedEventArgs e)
         *
         * @brief   Event handler. Called by button6 for click events.
         *          This functions sends the "#4" (Calibration) command to the LIDAR-Scanner.
         *
         * @author  Alexander Miller (7089316)
         * @date    22.12.2015
         *
         * @param   sender  Source of the event.
         * @param   e       Routed event information.
         **************************************************************************************************/

        private void button6_Click(object sender, RoutedEventArgs e)
        {
            ComPort.WriteLine("#4");
        }

        /**********************************************************************************************//**
         * @fn  private void neuMessung_Click(object sender, RoutedEventArgs e)
         *
         * @brief   Event handler. Called by neuMessung for click events.
         *          This function adds a new Measurement to MeasureList and refreshes combobox2.
         *          1. Add new Measurement to MeasureList  
         *          2. Clear combobox2  
         *          3. Add all Measurements to combobox2  
         *          4. Display the new Measurement
         *
         * @author  Alexander Miller (7089316)
         * @date    22.12.2015
         *
         * @param   sender  Source of the event.
         * @param   e       Routed event information.
         **************************************************************************************************/

        private void neuMessung_Click(object sender, RoutedEventArgs e)
        {
            //1.
            Measurement mhelper = new Measurement(new Vector3D(), 0, 0);
            MeasureList.Add(mhelper);
            //2.
            comboBox2.Items.Clear();
            //3.
            foreach (Measurement x in MeasureList)
            {
                comboBox2.Items.Add(x);
            }
            comboBox2.SelectedIndex = 0;
            //4.
            myViewport.Children.Add(mhelper.getGeometry3D());

        }

        /**********************************************************************************************//**
         * @fn  private void entfMessung_Click(object sender, RoutedEventArgs e)
         *
         * @brief   Event handler. Called by entfMessung for click events.
         *          This functions deletes the selected measurement and updates combobox2.
         *          1. Check if at least 2 measurements are available  
         *          2. Remove the selected measurement form MeasureList  
         *          3. Clear combobox2  
         *          4. Add all remaining measurements to combobox2  
         *          5. Reset myViewport  
         *          6. Add all remaining measurements to myViewport as children to display them
         *
         * @author  Alexander Miller (7089316)
         * @date    22.12.2015
         *
         * @param   sender  Source of the event.
         * @param   e       Routed event information.
         **************************************************************************************************/

        private void entfMessung_Click(object sender, RoutedEventArgs e)
        {
            //1
            if (MeasureList.Count > 1)
            {
                //2
                MeasureList.RemoveAt(selectedMeasure);
                //3
                comboBox2.Items.Clear();
                //4
                foreach (Measurement x in MeasureList)
                {
                    comboBox2.Items.Add(x);
                }
                comboBox2.SelectedIndex = 0;
                //5
                myViewport.Children.Clear();
                //6
                myViewport.Children.Add(gridLinesXY);
                myViewport.Children.Add(new DefaultLights());
                foreach (Measurement x in MeasureList)
                {
                    myViewport.Children.Add(x.getGeometry3D());
                }
            }


        }

        /**********************************************************************************************//**
         * @fn  private void offsetBtn_Click(object sender, RoutedEventArgs e)
         *
         * @brief   Event handler. Called by offsetBtn for click events.
         *          This function gets the offset information. Then id adds this values to the selected measurement.
         *          
         *          a. Try to parse all values    
         *          
         *              1.  Set all offset values   
         *           
         *          b. If parsing fails show a MessageBox.
         *
         * @author  Alexander Miller (7089316)
         * @date    22.12.2015
         *
         * @param   sender  Source of the event.
         * @param   e       Routed event information.
         **************************************************************************************************/

        private void offsetBtn_Click(object sender, RoutedEventArgs e)
        {
            double x, y, z, degX ,degZ = 0;
            //a
            if (double.TryParse(xOffsetTxt.Text, out x) &&
                double.TryParse(yOffsetTxt.Text, out y) &&
                double.TryParse(zOffsetTxt.Text, out z) &&
                double.TryParse(degOffsetXTxt.Text, out degX) &&
                double.TryParse(degOffsetZTxt.Text, out degZ)
                )
            {
                //1
                MeasureList[selectedMeasure].linearOffset = new Vector3D(x, y, z);
                MeasureList[selectedMeasure].rotaryOffsetX = degX;
                MeasureList[selectedMeasure].rotaryOffsetZ = degZ;
                MeasureList[selectedMeasure].Refresh();

            }
            //b
            else
            {
                MessageBox.Show("Für den Offset sind nur Zahlenwerte erlaubt.", "Fehleingabe erkannt!", MessageBoxButton.OK, MessageBoxImage.Information);
            }
        }

        /**********************************************************************************************//**
         * @fn  private void saveBtn_Click(object sender, RoutedEventArgs e)
         *
         * @brief   Event handler. Called by saveBtn for click events.
         *          Calls "DAO.saveMeasurements" to save all measurements to a xml file.
         *
         * @author  Alexander Miller (7089316)
         * @date    22.12.2015
         *
         * @param   sender  Source of the event.
         * @param   e       Routed event information.
         **************************************************************************************************/

        private void saveBtn_Click(object sender, RoutedEventArgs e)
        {
            DAO.saveMeasurements(MeasureList);
        }

        /**********************************************************************************************//**
         * @fn  private void loadBtn_Click(object sender, RoutedEventArgs e)
         *
         * @brief   Event handler. Called by loadBtn for click events.
         *          Calls "DAO.loadMeasurements" to load measurements from a xml file.
         *          1. Get a list of Measurements from a xml file    
         *          2. Clear combobox2  
         *          3. Add all measurements to combobox2  
         *          4. Reset myViewport  
         *          5. Add all measurements to myViewport as children to display them 
         *
         * @author  Alexander Miller (7089316)
         * @date    22.12.2015
         *
         * @param   sender  Source of the event.
         * @param   e       Routed event information.
         **************************************************************************************************/

        private void loadBtn_Click(object sender, RoutedEventArgs e)
        {
            //1.
            List<Measurement> loaded = DAO.loadMeasurements();
            if (loaded != null)
            {
                MeasureList = loaded;
                //2.
                comboBox2.Items.Clear();
                //3.
                foreach (Measurement x in MeasureList)
                {
                    comboBox2.Items.Add(x);
                }
                comboBox2.SelectedIndex = 0;
                //4.
                myViewport.Children.Clear();
                //5.
                myViewport.Children.Add(gridLinesXY);
                myViewport.Children.Add(new DefaultLights());
                foreach (Measurement x in MeasureList)
                {
                    x.makeGeometry3D();
                    myViewport.Children.Add(x.getGeometry3D());
                }
            }
        }

        /**********************************************************************************************//**
         * @fn  private void sendBtn_Click(object sender, RoutedEventArgs e)
         *
         * @brief   Event handler. Called by sendBtn for click events.
         *          This function sends the text of "sendTxt" to the LIDAR-Scanner.
         *
         * @author  Alexander Miller (7089316)
         * @date    22.12.2015
         *
         * @param   sender  Source of the event.
         * @param   e       Routed event information.
         **************************************************************************************************/

        private void sendBtn_Click(object sender, RoutedEventArgs e)
        {

            ComPort.Write(sendTxt.Text);

        }

        /**********************************************************************************************//**
         * @fn  private void enableVisuals(bool b)
         *
         * @brief   Enables/Disables the visuals.
         *
         * @author  Alexander Miller (7089316)
         * @date    22.12.2015
         *
         * @param   b   true enables visuals and false disables them.
         **************************************************************************************************/

        private void enableVisuals(bool b)
        {
            //neuMessung.IsEnabled = b;
            //entfMessung.IsEnabled = b;
            //offsetBtn.IsEnabled = b;
            btn_einzelmessung.IsEnabled = b;
            btn_kalibrieren.IsEnabled = b;
            posBtn.IsEnabled = b;
            btn_radar.IsEnabled = b;
            txt_MPos.IsEnabled = b;
            txt_SPos.IsEnabled = b;
            //sendBtn.IsEnabled = b;
            //comboBox2.IsEnabled = b;
            //sendTxt.IsEnabled = b;
            //txt_punktmessung.IsEnabled = b;
            //xOffsetTxt.IsEnabled = b;
            //yOffsetTxt.IsEnabled = b;
            //zOffsetTxt.IsEnabled = b;
            //degOffsetTxt.IsEnabled = b;
            //textBox1.IsEnabled = b;
        }


        /**********************************************************************************************//**
         * @fn  private void reverse_Click(object sender, RoutedEventArgs e)
         *
         * @brief   Event handler. Called by reverse for click events.
         *          This function transforms the 3D model to a open/closed shape.
         *
         * @author  Alex
         * @date    18.01.2016
         *
         * @param   sender  Source of the event.
         * @param   e       Routed event information.
         **************************************************************************************************/

        private void reverse_Click(object sender, RoutedEventArgs e)
        {
            MeasureList[selectedMeasure].open = !MeasureList[selectedMeasure].open;
            MeasureList[selectedMeasure].Refresh();
        }
    }
}
