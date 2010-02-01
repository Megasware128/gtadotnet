using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GTA
{
    public class Draw
    {
        public static void Corona(Vector3 position, GTAColor color)
        {
            Corona(position, color, 1.0f);
        }

        public static void Corona(Vector3 position, GTAColor color, float radius)
        {
            Corona(position, color, radius, 0);
        }

        public static void Corona(Vector3 position, GTAColor color, float radius, int type)
        {
            Corona(position, color, radius, type, false);
        }

        public static void Corona(Vector3 position, GTAColor color, float radius, int type, bool lensFlare)
        {
            Internal.Function.Call(0x024f, position, radius, type, lensFlare, color.R, color.G, color.B);
        }
    }
}
