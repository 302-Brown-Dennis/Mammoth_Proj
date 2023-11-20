// Lag Compensation class currently a WIP
#pragma once


class LagCompensation
{
public:
	void ServerRewind();
private:
protected:
	void CalculateFrame();
	void SaveHitBox();
	void MoveHitBox();
	void ResetHitBox();
	void SaveFrame();
	void GetFrame();
	void ServerRewind();

	

};