#pragma once
class SceneObj
{
public:
	SceneObj();
	
	~SceneObj();
private:
	virtual void init() = 0;
};

