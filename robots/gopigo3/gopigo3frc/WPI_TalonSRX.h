#pragma once

#include "SpeedController.h"
#include <string>

namespace ctre 
{
	namespace phoenix 
	{
		namespace motorcontrol 
		{
			enum class ControlMode
			{
				PercentOutput = 0,
				Position = 1,
				Velocity = 2,
				Current = 3,
				Follower = 5,
				MotionProfile = 6,
				MotionMagic = 7,
				MotionMagicArc = 8,
				//TimedPercentOutput = 9,
				MotionProfileArc = 10,
				Disabled = 15,

			};
			
			namespace can
			{
				class WPI_TalonSRX : public frc::SpeedController
				{
				public:
					WPI_TalonSRX(int devicenumber);
					virtual ~WPI_TalonSRX();

					WPI_TalonSRX() = delete;
					WPI_TalonSRX(WPI_TalonSRX const &) = delete;
					WPI_TalonSRX& operator=(WPI_TalonSRX const &) = delete;

					virtual void Set(double speed);
					virtual void PIDWrite(double output);

					virtual double Get() const;

					virtual void Set(ControlMode mode, double value);
					virtual void Set(ControlMode mode, double demand0, double demand1);

					virtual void SetInverted(bool inverted)
					{
						m_inverted = inverted;
					}

					virtual bool GetInverted() const
					{
						return m_inverted;
					}

					virtual void Disable()
					{
						m_mode = ControlMode::Disabled;
					}

					virtual void StopMotor();

					void SetExpiration(double timeout)
					{
						m_timeout = timeout;
					}

					double GetExpiration() const
					{
						return m_timeout;
					}

					bool IsAlive() const;

					bool IsSafteyEnabled() const
					{
						return m_safety;
					}

					void SetSafteyEnabled(bool enabled)
					{
						m_safety = enabled;
					}

					void GetDescription(std::string &desc)
					{
						desc = m_desc;
					}

				private:
					ControlMode m_mode;
					std::string m_desc;
					bool m_inverted;
					bool m_timeout;
					bool m_safety;
				};
			}
		}
	}
}

