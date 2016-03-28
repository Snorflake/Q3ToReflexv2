#include "entity.h"

const char *entity_base::typenames[] =
{
	"Vector3",
	"ColourARGB32",
	"ColourXRGB32",
	"String32",
	"String64",
	"String256",
	"UInt64",
	"Int32",
	"UInt32",
	"Int16",
	"UInt16",
	"UInt8",
	"Int8",
	"Float",
	"Bool8"
};

#define SPECIALIZE_PRINT_VAL(TYPE, FORMAT, ...)	\
template<> void entity_base::print_val<TYPE>(FILE *outfile, TYPE&& val)	\
{	\
	fprintf(outfile, FORMAT, ##__VA_ARGS__);	\
}

SPECIALIZE_PRINT_VAL(int64_t, "%lli", val)
SPECIALIZE_PRINT_VAL(uint64_t, "%llu", val)
SPECIALIZE_PRINT_VAL(float, "%.6f", val)
SPECIALIZE_PRINT_VAL(std::string, "%s", val.c_str())
SPECIALIZE_PRINT_VAL(bool, "%u", val ? 1 : 0)
SPECIALIZE_PRINT_VAL(vec3_t, "%.6f %.6f %.6f", val[0], val[1], val[2])

void entity_base::brush::face::print(FILE *outfile) const
{
	fprintf(outfile, "\t\t\t%.6f %.6f %.6f %.6f %.6f ", wtf[0], wtf[1], wtf[2], wtf[3], wtf[4]);
	for (auto&& v : vertices)
	{
		fprintf(outfile, "%u ", *v);
	}
	fprintf(outfile, "0x%08x %s\n", color, material.c_str());
}

void entity_base::brush::face::add_vertex(float x, float y, float z)
{

}

entity_base::brush::brush() : vertices([](const vec3_t*& a, const vec3_t*& b) { return (a[0] == b[0]) ? (a[1] == b[1]) ? (a[2] < b[2]) : (a[1] < b[1]) : (a[0] < b[0]); })
{
}

entity_base::brush::~brush()
{
	for (auto&& f : faces)
	{
		delete f;
	}
}

entity_base::brush::face& entity_base::brush::new_face(const std::string& material, uint32_t color)
{
	face *f = new face(material, color);
	faces.push_back(f);
	return *f;
}

entity_base::brush::face& entity_base::brush::new_face(std::string&& material, uint32_t color)
{
	face *f = new face(material, color);
	faces.push_back(f);
	return *f;
}

entity_base::brush::face& entity_base::brush::new_face(uint32_t color)
{
	face *f = new face("", color);
	faces.push_back(f);
	return *f;
}

void entity_base::brush::print(FILE *outfile)
{
	fprintf(outfile, "\tbrush\n\t\tvertices\n");
	uint8_t vertex_count = 0;
	for (auto&& v : vertices)
	{
		v.second = vertex_count++;
		fprintf(outfile, "\t\t\t");
		fprintf(outfile, "\t\t\t%.6f %.6f %.6f", v.first[0], v.first[1], v.first[2]);
	}
	fprintf(outfile, "\t\tfaces\n");
	for (auto&& f : faces)
	{
		f->print(outfile);
	}
}

entity_base::entity_base(entity_type_t type)
{
	property<UInt8> *p = new property<UInt8>("type");
	p->val = type;
	properties.emplace_back(p);
}

entity_base::~entity_base()
{
	for (auto&& prop : properties)
	{
		delete prop;
	}

	for (auto&& brush : brushes)
	{
		delete brush;
	}
}

entity_base::brush& entity_base::new_brush()
{
	brush *b = new brush;
	brushes.push_back(b);
	return *b;
}

void entity_base::print(FILE *outfile) const
{
	fprintf(outfile, "\tentity\n");
	for (auto&& prop : properties)
	{
		prop->print(outfile);
	}
	for (auto&& brush : brushes)
	{
		brush->print(outfile);
	}
}
