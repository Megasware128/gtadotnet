using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GTA
{
    public enum CameraLookMode
    {
        Normal = 2,
        Transverse = 1
    }

    public class Camera
    {
        public static int TransverseDelay
        {
            set
            {
                Internal.Function.Call(0x0460, 0.0f, value);
            }
        }

        public static void SetPosition(Vector3 position, Vector3 rotation)
        {
            Internal.Function.Call(0x015f, position, rotation);
        }

        public static void LookAt(Vector3 position, CameraLookMode mode)
        {
            Internal.Function.Call(0x0160, position, (int)mode);
        }

        public static void Restore()
        {
            Restore(false);
        }

        public static void Restore(bool instantly)
        {
            if (instantly)
            {
                Internal.Function.Call(0x02eb);
            }
            else
            {
                Internal.Function.Call(0x015a);
            }
        }

        public static bool IsSphereVisible(Vector3 position, float radius)
        {
            return Internal.Function.Call(0x00c2, position, radius);
        }
    }
}
