#pragma once
#include "MicroToolKit.h"
#include "Interfaces/IHashable.h"
#include "Utility/Sort.h"

//#define DEBUG

using namespace mtk;

struct Vector3 : public IHashable<>
{
	float x = 0.0f, y = 0.0f, z = 0.0f;

	Vector3()
	{
	}

	Vector3(float _scalar)
		: x(_scalar), y(_scalar), z(_scalar)
	{
	}

	Vector3(const float& _x, const float& _y, const float& _z)
		: x(_x), y(_y), z(_z)
	{
	}

	Vector3(const Vector3& _other)
		: x(_other.x), y(_other.y), z(_other.z)
	{
#ifdef DEBUG
		LOG("Copyed! Vec");
#endif
	}

	Vector3(Vector3&& _other) noexcept
		: x(_other.x), y(_other.y), z(_other.z)
	{
#ifdef DEBUG
		LOG("Moved! Vec");
#endif
	}

	~Vector3()
	{
#ifdef DEBUG
		LOG("Destroyed! Vec");
#endif
	}

	newhash_t GetHash() const override
	{
		newhash_t size = (newhash_t)(sizeof(Vector3) + sizeof(newhash_t) + x - y * z);
		newhash_t factor = (newhash_t)((float)size - y + ((float)size * 0.5f));
		return factor;
	}

	Vector3& operator=(Vector3&& _other) noexcept
	{
		x = _other.x;
		y = _other.y;
		z = _other.z;

#ifdef DEBUG
		LOG("Moved! Vec");
#endif

		return *this;
	}

	Vector3& operator=(const Vector3& _other)
	{
		x = _other.x;
		y = _other.y;
		z = _other.z;

#ifdef DEBUG
		LOG("Copyed! Vec");
#endif

		return *this;
	}

	bool operator==(const Vector3& _other)
	{
		return x == _other.x && y == _other.y && z == _other.z;
	}

	friend bool operator==(const Vector3& _current, const Vector3& _other)
	{
		return _current.x == _other.x && _current.y == _other.y && _current.z == _other.z;
	}

	friend bool operator<=(const Vector3& _current, const Vector3& _other)
	{
		return _current.x <= _other.x && _current.y <= _other.y && _current.z <= _other.z;
	}

	friend bool operator>=(const Vector3& _current, const Vector3& _other)
	{
		return _current.x >= _other.x && _current.y >= _other.y && _current.z >= _other.z;
	}

	friend bool operator<(const Vector3& _current, const Vector3& _other)
	{
		return _current.x < _other.x && _current.y < _other.y && _current.z < _other.z;
	}

	friend bool operator>(const Vector3& _current, const Vector3& _other)
	{
		return _current.x > _other.x && _current.y > _other.y && _current.z > _other.z;
	}

	friend std::ostream& operator<<(std::ostream& _stream, const Vector3& _current)
	{
		_stream << "[ " << _current.x << ", " << _current.y << ", " << _current.z << " ]";
		return _stream;
	}
};

class Player : public IHashable<>
{
public:
	enum Class
	{
		Warrior = 0,
		Mage,
		Assassin,
		Guardian,
		Hunter
	};

public:
	Player()
		: m_Name("New Player"), m_Level(0), m_Class(Warrior), m_Position(0, 0, 0)
	{
	}

	Player(uint32_t _level)
		: m_Name("New Player"), m_Level(_level), m_Class(Warrior), m_Position(0, 0, 0)
	{
	}

	Player(const char* _name)
		: m_Name(_name), m_Level(1), m_Class(Warrior), m_Position(0, 0, 0)
	{
	}

	Player(const char* _name, uint32_t _level)
		: m_Name(_name), m_Level(_level), m_Class(Warrior), m_Position(0, 0, 0)
	{
	}

	Player(const char* _name, Class _class)
		: m_Name(_name), m_Level(1), m_Class(_class), m_Position(0, 0, 0)
	{
	}

	Player(const char* _name, uint32_t _level, Class _class)
		: m_Name(_name), m_Level(_level), m_Class(_class), m_Position(0, 0, 0)
	{
	}

	Player(const Player& _other)
		: m_Name(_other.m_Name), m_Level(_other.m_Level), m_Class(_other.m_Class), m_Position(_other.m_Position)
	{
#ifdef DEBUG
		LOG("Copyed! Play");
#endif
	}

	Player(Player&& _other) noexcept
		: m_Name(_other.m_Name), m_Level(_other.m_Level), m_Class(_other.m_Class), m_Position(std::move(_other.m_Position))
	{
#ifdef DEBUG
		LOG("Moved! Play");
#endif
	}

	~Player()
	{
#ifdef DEBUG
		LOG("Destroyed! Play");
#endif
	}

	const char* GetName() const { return m_Name; }
	uint32_t GetLevel() const { return m_Level; }
	Class GetClass() const { return m_Class; }
	Vector3 GetPosition() const { return m_Position; }

	newhash_t GetHash() const override
	{
		newhash_t size = (newhash_t)(sizeof(Player) + sizeof(newhash_t)) + m_Level;
		newhash_t factor = (newhash_t)((float)size - (float)m_Class + ((float)size * 0.5f));
		return factor;
	}

	Player& operator=(const Player& _other)
	{
		m_Name = _other.m_Name;
		m_Level = _other.m_Level;
		m_Class = _other.m_Class;
		m_Position = _other.m_Position;
#ifdef DEBUG
		LOG("Copyed! Play");
#endif
		return *this;
	}

	Player& operator=(Player&& _other) noexcept
	{
		m_Name = _other.m_Name;
		m_Level = _other.m_Level;
		m_Class = _other.m_Class;
		m_Position = std::move(_other.m_Position);
#ifdef DEBUG
		LOG("Moved! Play");
#endif
		return *this;
	}

	bool operator==(const Player& _other)
	{
		return m_Level == _other.m_Level && m_Class == _other.m_Class && m_Position == _other.m_Position;
	}

	friend bool operator==(const Player& _current, const Player& _other)
	{
		return _current.m_Level == _other.m_Level && _current.m_Class == _other.m_Class && _current.m_Position == _other.m_Position;
	}

	friend bool operator<=(const Player& _current, const Player& _other)
	{
		return _current.m_Level <= _other.m_Level && _current.m_Class <= _other.m_Class && _current.m_Position <= _other.m_Position;
	}

	friend bool operator>=(const Player& _current, const Player& _other)
	{
		return _current.m_Level >= _other.m_Level && _current.m_Class >= _other.m_Class && _current.m_Position >= _other.m_Position;
	}

	friend bool operator<(const Player& _current, const Player& _other)
	{
		return _current.m_Level < _other.m_Level && _current.m_Class < _other.m_Class && _current.m_Position < _other.m_Position;
	}

	friend bool operator>(const Player& _current, const Player& _other)
	{
		return _current.m_Level > _other.m_Level && _current.m_Class > _other.m_Class && _current.m_Position > _other.m_Position;
	}

	friend std::ostream& operator<<(std::ostream& _stream, const Player& _current)
	{
		_stream << _current.m_Name << " [ Class: " << _current.m_Class << " Level: " << _current.m_Level << " ]";
		return _stream;
	}

protected:
	const char* m_Name;
	uint32_t m_Level;
	Class m_Class;
	Vector3 m_Position;
};

template<typename _Type>
newhash_t MDTHash(const _Type& _obj, size_t _capacity)
{
	return {};
}

template<>
newhash_t MDTHash(const Player& _obj, size_t _capacity)
{
	newhash_t size = (newhash_t)(sizeof(Player) + sizeof(newhash_t)) + _obj.GetLevel();
	newhash_t factor = (newhash_t)((float)size - (float)_obj.GetClass() + ((float)size * 0.5f));
	return factor % _capacity;
}

template<>
newhash_t MDTHash(const Vector3& _obj, size_t _capacity)
{
	newhash_t size = (newhash_t)(sizeof(Vector3) + sizeof(newhash_t) + _obj.x - _obj.y * _obj.z);
	newhash_t factor = (newhash_t)((float)size - (float)_obj.y + ((float)size * 0.5f));
	return factor % _capacity;
}

namespace std {

	template<>
	struct hash<Player>
	{
		std::size_t operator()(const Player& _player) const
		{
			return _player.GetHash();
		}
	};

	template<>
	struct hash<Vector3>
	{
		std::size_t operator()(const Vector3& _vector) const
		{
			return _vector.GetHash();
		}
	};

}