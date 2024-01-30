#pragma once

template<typename T>
class Singleton
{
public:
	// インスタンス取得
	static T* GetInstance() {
		static T instance;
		return &instance;
	}
protected:
	Singleton() = default;
	virtual ~Singleton() = default;

	// コピー禁止
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

};
