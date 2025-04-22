#pragma once

template <typename T>
class Singleton {
public:
	static T& Get() 
	{
		static T s_Instance;

		return s_Instance;
	}

protected:
	Singleton() = default;
	virtual ~Singleton() = default;

	// 복사 및 이동 금지
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;
};