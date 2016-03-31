#pragma once

#ifndef ENTITY_H
#define ENTITY_H

#include "ctordefs.h"

#include <stdio.h>
#include <stdint.h>
#include <tuple>
#include <list>
#include <vector>
#include <map>
#include <functional>

typedef float vec3_t[3];

enum entity_type_t : uint8_t
{
	WorldSpawn,
	PlayerSpawn,
	Player,
	PointLight,
	Grenade,
	Rocket,
	Cell,
	Burst,
	Stake,
	Teleporter,
	Target,
	JumpPad,
	Effect,
	Pickup,
	Temp,
	CameraPath,
	Vote,
	Damage,
	RaceStart,
	RaceFinish,
	WeaponRestrictor,
	Prefab,
	VolumeSelect,
	WorkshopScreenshot,
	ReflectionProbe
};

class entity_base
{
protected:

	enum property_type_t : uint8_t
	{
		Vector3,
		ColourARGB32,
		ColourXRGB32,
		String32,
		String64,
		String256,
		UInt64,
		Int32,
		UInt32,
		Int16,
		UInt16,
		UInt8,
		Int8,
		Float,
		Bool8,
	};

	static const char *typenames[15];
	static const char *entity_typenames[25];

private:
	template<typename T> static inline void print_val(FILE *outfile, T&& val);
	//template<typename T> static inline void print_val(FILE *outfile, const T& val);

	class property_base
	{
	public:
		virtual inline void print(FILE *outfile) const = 0;
		virtual ~property_base() {}
	};

public:

	class brush
	{
		friend class entity_base;

	public:

		class vertex
		{
			friend class brush;
			float x;
			float y;
			float z;
		public:
			vertex(const vertex& v);
			vertex(float x, float y, float z);
			vertex& operator= (const vertex& v);
			bool operator== (const vertex& v) const;
			bool operator< (const vertex& v) const;
		};

		class face
		{
			friend class brush;

			brush						&parent;

			float						wtf[5];		// pls
			std::vector<const uint8_t*>	vertices;
			uint32_t					color;
			std::string					material;

			face();

			inline void print(FILE *outfile) const;
			template<typename S> face(brush& parent, S&& material, uint32_t color) : parent(parent), material(material), color(color)
			{
				wtf[0] = 0;
				wtf[1] = 0;
				wtf[2] = 1;
				wtf[3] = 1;
				wtf[4] = 0;
			}
		public:
			void add_vertex(float x, float y, float z);
			NO_COPY_OR_MOVE_CTOR(face)
		};

	private:

		std::map<vertex, uint8_t>	vertices;
		std::vector<face*>	faces;
		
		brush();
		void print(FILE *outfile);
	public:
		~brush();
		face& new_face(const std::string& material, uint32_t color = 0);
		face& new_face(std::string&& material, uint32_t color = 0);
		face& new_face(uint32_t color = 0);
		NO_COPY_OR_MOVE_CTOR(brush)
	};

protected:

	std::list<property_base*>	properties;
	std::list<brush*>			brushes;

	template<property_type_t type> class property : public property_base
	{
		template<size_t N> using property_type = std::tuple_element_t<N, std::tuple<
			vec3_t,
			uint32_t,
			uint32_t,
			char[32],
			char[64],
			char[256],
			uint64_t,
			int32_t,
			uint32_t,
			int16_t,
			uint16_t,
			uint8_t,
			int8_t,
			float,
			uint8_t>>;

		template<size_t N> using property_print_type = std::tuple_element_t<N, std::tuple<
			float[3],
			uint64_t,
			uint64_t,
			std::string,
			std::string,
			std::string,
			uint64_t,
			int64_t,
			uint64_t,
			int64_t,
			uint64_t,
			uint64_t,
			int64_t,
			float,
			bool>>;

		const std::string key;
	public:
		property_type<type> val;
		template<typename S> property(S&& key) : key(std::forward<std::string>(key)) {}
		virtual inline void print(FILE *outfile) const
		{
			fprintf(outfile, "\t\t%s %s ", typenames[type], key.c_str());
			print_val(outfile, property_print_type<type>(val));
			fprintf(outfile, "\n");
		}
		NO_COPY_OR_MOVE_CTOR(property)
	};
	const entity_type_t type;
	entity_base(entity_type_t type);
	virtual ~entity_base();
public:
	brush& new_brush();
	void print(FILE *outfile) const;

	NO_COPY_OR_MOVE_CTOR(entity_base)
};

template<entity_type_t type> class entity : public entity_base
{
public:
	entity() : entity_base(type) {}
};

#endif
