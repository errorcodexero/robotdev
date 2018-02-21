using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Diagnostics;
using SharpDX.DirectInput;

namespace GoDriverStation
{
    class JoystickManager
    {
        #region private members
        /// <summary>
        /// The direct input object
        /// </summary>
        private DirectInput m_input;

        /// <summary>
        /// The list of joysticks and gamepads
        /// </summary>
        private IList<GamepadDesc> m_device_descs;
        #endregion
        
        #region public constructors
        public JoystickManager()
        {
            m_device_descs = new List<GamepadDesc>();
            m_input = new DirectInput();

            foreach (DeviceInstance inst in m_input.GetDevices(DeviceType.Joystick, DeviceEnumerationFlags.AllDevices))
            {
                m_device_descs.Add(new GamepadDesc(inst.InstanceName, inst.InstanceGuid));
            }

            foreach (DeviceInstance inst in m_input.GetDevices(DeviceType.Gamepad, DeviceEnumerationFlags.AllDevices))
            {
                m_device_descs.Add(new GamepadDesc(inst.InstanceName, inst.InstanceGuid));
            }

            bool b = true;
            Joystick j = new Joystick(m_input, m_device_descs[0].GUID);
            ConstantForce f = new ConstantForce();
            while (true)
            {
                Thread.Sleep(3000);
                if (b)
                {
                    j.Acquire();
                    j.SendForceFeedbackCommand(f);
                }

                b = !b;
            }
        }
        #endregion

        #region public methods
        #endregion

    }
}
