/**********************************************************************************************//**
 * @file    dao.cs
 *
 * @brief   Implements the dao class.
 **************************************************************************************************/

using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Xml;
using System.Xml.Serialization;


namespace LIDAR_Controller
{
    /**********************************************************************************************//**
     * @class   DAO
     *
     * @brief   A "data access object" class.
     *          This is used to separate the file operations from the GUI logic.
     *
     * @author  Alexander Miller (7089316)
     * @date    22.12.2015
     **************************************************************************************************/

    class DAO
    {
        /**********************************************************************************************//**
         * @fn  public static List<Measurement> loadMeasurements()
         *
         * @brief   Loads the measurements from a xml file.
         *          First ssk user if he/she wants to load data. If not return null.  
         *          
         *          a: If no problems occur then...   
         *          
         *              1. Open a file selector  
         *              2. If a file was selected, open it  
         *              3. Create a XmlSerializer  
         *              4. Deserialize the data into a list  
         *              5. Get the maximum measurement id and set it  
         *              6. Close the file  
         *              7. Inform user about successfully loading all data  
         *          
         *          b: If something goes wrong, show a MessageBox.             
         *
         * @author  Alexander Miller (7089316)
         * @date    22.12.2015
         *
         * @return  A list of all measurements.
         **************************************************************************************************/

        public static List<Measurement> loadMeasurements()
        {
            if (MessageBox.Show("Möchten Sie Messergebnisse aus einer Datei laden?\nDabei gehen nicht gespeicherte Messungen verloren!", "Warnung!", MessageBoxButton.OKCancel, MessageBoxImage.Warning) == MessageBoxResult.OK)
            {
                List<Measurement> list;
                //a.
                try
                {
                    //1.
                    OpenFileDialog ofdialog = new OpenFileDialog();
                    ofdialog.DefaultExt = ".lmd"; //LIDAR Measurement Data
                    ofdialog.Filter = "LIDAR Measurement Data|*.lmd";
                    Nullable<bool> r = ofdialog.ShowDialog();
                    if (r == true)
                    {
                        //2.
                        FileStream fs = new FileStream(ofdialog.FileName, FileMode.Open);
                        //3.
                        Type t = typeof(List<Measurement>);
                        XmlSerializer serializer = new XmlSerializer(t);
                        //4.
                        list = (List<Measurement>)serializer.Deserialize(fs);
                        //5.
                        Measurement.id = list[list.Count-1].mId+1;
                        //6.
                        fs.Close();
                        //7.
                        MessageBox.Show("Laden erfolgreich.", "Info", MessageBoxButton.OK, MessageBoxImage.Information);
                        return list;
                    }

                }
                //b.
                catch (Exception E)
                {
                    MessageBox.Show(E.Message, "Fehler!", MessageBoxButton.OK, MessageBoxImage.Error);
                }
            }
            return null;
            }

        /**********************************************************************************************//**
         * @fn  public static void saveMeasurements(List<Measurement> l)
         *
         * @brief   Saves the measurements to a xml file.  
         *          
         *          a. If no problems occur then...   
         *          
         *              1. Open a file selector  
         *              2. If a file was selected, create it  
         *              3. Create a XmlSerializer  
         *              4. Serialize the data into the file  
         *              5. Close the file  
         *              6. Inform user about successfully saving all data  
         *              
         *          b. If something goes wrong, show a MessageBox.  
         *
         * @author  Alexander Miller (7089316)
         * @date    22.12.2015
         *
         * @param   l   The List&lt;Measurement&gt; to process.
         **************************************************************************************************/

        public static void saveMeasurements(List<Measurement> l)
        {
            //a.
            try
            {
                //1.
                SaveFileDialog sfdialog = new SaveFileDialog();
                sfdialog.DefaultExt = ".lmd"; //LIDAR Measurment Data
                sfdialog.Filter = "LIDAR Measurment Data|*.lmd";
                Nullable<bool> r = sfdialog.ShowDialog();
                if (r == true)
                {
                    XmlWriterSettings xmlWriterSettings = new XmlWriterSettings();
                    xmlWriterSettings.Indent = true;
                    //2.
                    FileStream fs = new FileStream(sfdialog.FileName, FileMode.Create);
                    //3.
                    Type t = typeof(List<Measurement>);
                    XmlSerializer serializer = new XmlSerializer(t);
                    //4.
                    XmlWriter xmlWriter = XmlWriter.Create(fs, xmlWriterSettings);
                    serializer.Serialize(xmlWriter, l);
                    //5.
                    fs.Close();
                    //6.
                    MessageBox.Show("Speichern erfolgreich.", "Info", MessageBoxButton.OK, MessageBoxImage.Information);
                }
                
            }
            //b.
            catch (Exception E)
            {
                MessageBox.Show(E.Message, "Fehler!", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

    }



}
