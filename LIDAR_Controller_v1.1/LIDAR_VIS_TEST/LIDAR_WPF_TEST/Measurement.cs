/**********************************************************************************************//**
 * @file    measurement.cs
 *
 * @brief   Implements the measurement class.
 **************************************************************************************************/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Media3D;


namespace LIDAR_Controller
{
    /**********************************************************************************************//**
     * @class   Measurement
     *
     * @brief   (Serializable) a measurement.
     *
     * @author  Alexander Miller (7089316)
     * @date    22.12.2015
     **************************************************************************************************/

    [Serializable()]
    public class Measurement
    {
        /**********************************************************************************************//**
         * @brief   The maximum motor position.
         *
         **************************************************************************************************/

        public static int maxMPos = 200; //200 Steps 

        /**********************************************************************************************//**
         * @brief   The maximum servo position.
         *
         **************************************************************************************************/

        public static int maxSPos = 90; // 180 degrees

        /**********************************************************************************************//**
         * @property    public static int id
         *
         * @brief   Gets or sets the global measurement identifier.
         *
         * @return  The identifier.
         **************************************************************************************************/

        public static int id { get; set; }

        /**********************************************************************************************//**
         * @property    public int mId
         *
         * @brief   Gets or sets the local measurement identifier.
         *
         * @return  The measurement identifier.
         **************************************************************************************************/

        public int mId { get; set; }

        /**********************************************************************************************//**
         * @brief   Information describing the distance.
         *          This array gets filled by the LIDAR-Scanner.
         *
         **************************************************************************************************/

        public double[][] distanceData = new double[maxMPos+1][];

        /**********************************************************************************************//**
         * @brief   The origin of the measurement.
         *
         **************************************************************************************************/

        public Point3D origin = new Point3D(0, 0, 0);

        /**********************************************************************************************//**
         * @property    public Vector3D linearOffset
         *
         * @brief   Gets or sets the linear offset.
         *
         * @return  The linear offset in cm.
         **************************************************************************************************/

        public Vector3D linearOffset { get; set; }

        /**********************************************************************************************//**
         * @property    public double rotaryOffsetX
         *
         * @brief   Gets or sets the rotary offset around X.
         *
         * @return  The rotary offset in degrees.
         **************************************************************************************************/

        public double rotaryOffsetX { get; set; }

        /**********************************************************************************************//**
         * @property    public double rotaryOffsetZ
         *
         * @brief   Gets or sets the rotary offset around Z.
         *
         * @return  The rotary offset in degrees.
         **************************************************************************************************/

        public double rotaryOffsetZ { get; set; }


        /**********************************************************************************************//**
        * @brief   Generate a open or closed 3D model.
        *
        **************************************************************************************************/
        public bool open = true;

        /**********************************************************************************************//**
         * @brief   The measurement color.
         *
         **************************************************************************************************/

        public Color color;

        /**********************************************************************************************//**
         * @brief   The mesh that contains all 3D-Data.
         *
         **************************************************************************************************/

        MeshGeometry3D meshMain3D = new MeshGeometry3D();

        /**********************************************************************************************//**
         * @brief   The geometry data.
         *
         **************************************************************************************************/

        ModelVisual3D geometry3D = new ModelVisual3D();

        /**********************************************************************************************//**
         * @brief   The geometry model.
         *
         **************************************************************************************************/

        GeometryModel3D geometryModel3D = new GeometryModel3D();

        /**********************************************************************************************//**
         * @fn  public Measurement()
         *
         * @brief   Default constructor.
         *
         * @author  Alexander Miller (7089316)
         * @date    22.12.2015
         **************************************************************************************************/

        public Measurement() { }

        /**********************************************************************************************//**
         * @fn  public Measurement(Vector3D linearOffset, double rotaryOffsetX, double rotaryOffsetZ)
         *
         * @brief   Constructor.
         *          This constructor sets the initial offset values and generates a random color.
         *
         * @author  Alexander Miller (7089316)
         * @date    22.12.2015
         *
         * @param   linearOffset    The linear offset.
         * @param   rotaryOffsetX   The rotary offset around x.
         * @param   rotaryOffsetZ   The rotary offset around z.
         **************************************************************************************************/

        public Measurement(Vector3D linearOffset, double rotaryOffsetX, double rotaryOffsetZ)
        {
            this.linearOffset = linearOffset;
            origin.X = linearOffset.X;
            origin.Y = linearOffset.Y;
            origin.Z = linearOffset.Z;

            this.rotaryOffsetX = rotaryOffsetX;
            this.rotaryOffsetZ = rotaryOffsetZ;
            Random r = new Random();
            this.color = Color.FromArgb(150, (Byte)r.Next(0, 256), (Byte)r.Next(0, 256), (Byte)r.Next(0, 256));
            mId = id++;

            for (int i = 0; i < maxMPos+1; i++)
            {
                distanceData[i] = new double[maxSPos+1];
                for (int k = 0; k <= maxSPos; k++)
                { setDistanceData(i, k, 10); }
            }
            makeGeometry3D();
        }

        /**********************************************************************************************//**
         * @fn  public double getDistanceData(int mpos, int spos)
         *
         * @brief   Gets distance data.
         *
         * @author  Alexander Miller (7089316)
         * @date    22.12.2015
         *
         * @param   mpos    The motor position.
         * @param   spos    The servo position.
         *
         * @return  The distance data.
         **************************************************************************************************/

        public double getDistanceData(int mpos, int spos)
        {
            return distanceData[mpos][spos];
        }

        /**********************************************************************************************//**
         * @fn  public void setDistanceData(int mpos, int spos, int data)
         *
         * @brief   Sets distance data.
         *
         * @author  Alexander Miller (7089316)
         * @date    22.12.2015
         *
         * @param   mpos    The motor position.
         * @param   spos    The servo position.
         * @param   data    The data.
         **************************************************************************************************/

        public void setDistanceData(int mpos, int spos, int data)
        {
            distanceData[mpos][spos] = data;
        }

        /**********************************************************************************************//**
         * @fn  public void makeGeometry3D()
         *
         * @brief   Generates the 3D structure.
         *          1. Add origin as first point  
         *          2. Add all points that represent the distance  \n
         *              2.1. Turn the Vector of the actual point (distance value = x value) around the z-axis, x-axis and again around the z-axis.  \n 
         *              2.2. Add the rotated point to the model  \n
         *          3. Generate triangles to build the bottom/back layer
         *          4. Generate the area between bottom and back layer
         *
         * @author  Alexander Miller (7089316)
         * @date    22.12.2015
         **************************************************************************************************/

        public void makeGeometry3D()
        {
            //1.
            meshMain3D = new MeshGeometry3D();
            meshMain3D.Positions.Add(origin);
            //2.
            for (int k = 0; k <= maxSPos; k++)
            {
                for (int i = 0; i < maxMPos; i++)
                {
                    //2.1.
                    Vector3D v = Turn3DVektorXZY(new Vector3D(distanceData[i][k], 0, 0), i * (360.0 / maxMPos), k);
                    //2.2.
                    meshMain3D.Positions.Add(new Point3D(v.X + linearOffset.X, v.Y + linearOffset.Y, v.Z + linearOffset.Z));
                }
            }

            if (!open)
            {
                //3.
                for (int i = 0; i <= maxSPos; i += maxSPos)
                {
                    for (int k = 1; k <= maxMPos / 2; k++)
                    {
                        meshMain3D.TriangleIndices.Add(0);
                        meshMain3D.TriangleIndices.Add(k + (i * maxMPos));
                        meshMain3D.TriangleIndices.Add(k + 1 + (i * maxMPos));
                    }

                }
            }
            

            //4.
            for (int i = 0; i < maxSPos; i++)
            {
                for (int k = 1; k <= maxMPos / 2; k++)
                {
                    //left triangles
                    meshMain3D.TriangleIndices.Add(k + (i * maxMPos));
                    meshMain3D.TriangleIndices.Add(k + (i * maxMPos) + 1);
                    meshMain3D.TriangleIndices.Add(k + ((i + 1) * maxMPos));
                    //right triangles
                    meshMain3D.TriangleIndices.Add(k + ((i) * maxMPos) + 1);
                    meshMain3D.TriangleIndices.Add(k + ((i + 1) * maxMPos) + 1);
                    meshMain3D.TriangleIndices.Add(k + ((i + 1) * maxMPos));
                }
            }

            geometryModel3D.Geometry = meshMain3D;
            DiffuseMaterial matDiffuseMain = new DiffuseMaterial(new SolidColorBrush(color));
            geometryModel3D.Material = matDiffuseMain;
            geometryModel3D.BackMaterial = matDiffuseMain;
            geometry3D.Content = geometryModel3D;
        }

        /**********************************************************************************************//**
         * @fn  public ModelVisual3D getGeometry3D()
         *
         * @brief   Gets geometry data.
         *
         * @author  Alexander Miller (7089316)
         * @date    22.12.2015
         *
         * @return  The geometry data.
         **************************************************************************************************/

        public ModelVisual3D getGeometry3D()
        {
            return geometry3D;
        }

        /**********************************************************************************************//**
         * @fn  public void setGeometryPoint3D(int i, int k)
         *
         * @brief   Relocates a specific point.
         *          1. Get all points  
         *          2. Reset origin  
         *          3. Turn the Vector of the actual point (distance value = x value) around the z-axis, x-axis and again around the z-axis.  
         *          4. Set the point 
         *
         * @author  Alexander Miller (7089316)
         * @date    22.12.2015
         *
         * @param   i   Zero-based index of the point (and position of the motor).
         * @param   k   Position of the servo.
         **************************************************************************************************/

        public void setGeometryPoint3D(int i, int k)
        {
            //1.
            Point3DCollection points = meshMain3D.Positions;

            //2.
            points[0] = origin;

            //3.
            Vector3D v = Turn3DVektorXZY(new Vector3D(distanceData[i][k], 0, 0), i * (360.0 / maxMPos), k);
            
            //4.
            points[i + 1 + k * maxMPos] = new Point3D(v.X + linearOffset.X, v.Y + linearOffset.Y, v.Z + linearOffset.Z);

        }

        /**********************************************************************************************//**
         * @fn  private Vector3D Turn3DVektorXZY(Vector3D v, double mdegree, double sdegree)
         *
         * @brief   Turns a given X vector to the specified position.
         *          1. Transform degrees into rad  
         *          2. Turn the vector based on the values around xzy-axis.
         *
         * @author  Alexander Miller (7089316)
         * @date    22.12.2015
         *
         * @param   v       The Vector3D to process.
         * @param   mdegree The motor position in degrees.
         * @param   sdegree The servo position in degrees.
         *
         * @return  A Vector3D.
         **************************************************************************************************/

        private Vector3D Turn3DVektorXZY(Vector3D v, double mdegree, double sdegree)
        {
            //1.
            double alpha = (sdegree) * (Math.PI / 180); 
            double beta = (mdegree + rotaryOffsetZ) * (Math.PI / 180); 
            double gamma = (rotaryOffsetX) * (Math.PI / 180); 
            //2.
            Vector3D vn = new Vector3D();
            vn.X = v.X * Math.Cos(beta) * Math.Cos(alpha) - v.Y * Math.Sin(beta) + v.Z * Math.Cos(beta) * Math.Sin(alpha);
            vn.Y = v.X * (Math.Cos(gamma)*Math.Sin(beta)* Math.Cos(alpha) - Math.Sin(gamma)*Math.Sin(alpha)) + v.Y * Math.Cos(gamma) * Math.Cos(beta) + v.Z * (-Math.Cos(gamma) * Math.Sin(beta) * Math.Sin(alpha)-Math.Sin(gamma)*Math.Cos(alpha));
            vn.Z = v.X * (Math.Sin(gamma) * Math.Cos(alpha) + Math.Cos(gamma) * Math.Sin(alpha)) + v.Y * Math.Sin(gamma) * Math.Cos(beta) + v.Z * (-Math.Sin(gamma) *Math.Sin(alpha) + Math.Cos(gamma)*Math.Cos(alpha));
            return vn;
        }

        /**********************************************************************************************//**
         * @fn  public void Refresh()
         *
         * @brief   Refreshes this object.
         *
         * @author  Alexander Miller (7089316)
         * @date    22.12.2015
         **************************************************************************************************/

        public void Refresh()
        {
            makeGeometry3D();
            origin.X = linearOffset.X;
            origin.Y = linearOffset.Y;
            origin.Z = linearOffset.Z;
            for (int i = 0; i < maxMPos; i++)
            {
                for (int k = 0; k < maxSPos; k++)
                {
                    setGeometryPoint3D(i, k);
                }

            }
        }

        /**********************************************************************************************//**
         * @fn  public bool Equals(Measurement m)
         *
         * @brief   Tests if this Measurement is considered equal to another.
         *
         * @author  Alexander Miller (7089316)
         * @date    22.12.2015
         *
         * @param   m   The measurement to compare to this object.
         *
         * @return  true if the objects are considered equal, false if they are not.
         **************************************************************************************************/

        public bool Equals(Measurement m)
        {
            if (m == null) { return false; }
            if (m.mId == this.mId) { return true; }
            return false;
        }

    }
}
