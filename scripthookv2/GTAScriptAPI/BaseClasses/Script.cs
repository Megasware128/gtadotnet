using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GTA.Internal;

namespace GTA
{
    public class Script : BaseScript
    {
        public Player Player { get; set; }

        public override void OnStart() {
            Player = (Player)new Player(GlobalVariable.Get(2).Value);
            Player.Character = (Ped)new Ped(GlobalVariable.Get(3).Value);
        }
    }

    public class TickScript : Script
    {
        public int Interval { get; set; }
        public event EventHandler Tick;

        public TickScript()
        {
            Interval = 100;
        }

        public override void Run()
        {
            while (true)
            {
                ProcessKeyBindings();
                OnTick();

                Wait(Interval);
            }
        }

        public virtual void OnTick()
        {
            if (Tick != null)
            {
                Tick(this, EventArgs.Empty);
            }
        }
    }
}
