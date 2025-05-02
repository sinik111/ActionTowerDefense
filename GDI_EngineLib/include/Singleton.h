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

	// ���� �� �̵� ����
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;
};