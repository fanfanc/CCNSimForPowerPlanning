//
// Generated file, do not edit! Created by opp_msgc 4.2 from messages/ccn_data.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "ccn_data_m.h"

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




Register_Class(ccn_data);

ccn_data::ccn_data(const char *name, int kind) : cMessage(name,kind)
{
    this->chunk_var = 0;
    this->d_var = 0;
    this->hops_var = 0;
    this->repository_var = 0;
}

ccn_data::ccn_data(const ccn_data& other) : cMessage(other)
{
    copy(other);
}

ccn_data::~ccn_data()
{
}

ccn_data& ccn_data::operator=(const ccn_data& other)
{
    if (this==&other) return *this;
    cMessage::operator=(other);
    copy(other);
    return *this;
}

void ccn_data::copy(const ccn_data& other)
{
    this->chunk_var = other.chunk_var;
    this->d_var = other.d_var;
    this->hops_var = other.hops_var;
    this->repository_var = other.repository_var;
}

void ccn_data::parsimPack(cCommBuffer *b)
{
    cMessage::parsimPack(b);
    doPacking(b,this->chunk_var);
    doPacking(b,this->d_var);
    doPacking(b,this->hops_var);
    doPacking(b,this->repository_var);
}

void ccn_data::parsimUnpack(cCommBuffer *b)
{
    cMessage::parsimUnpack(b);
    doUnpacking(b,this->chunk_var);
    doUnpacking(b,this->d_var);
    doUnpacking(b,this->hops_var);
    doUnpacking(b,this->repository_var);
}

unsigned long ccn_data::getChunk() const
{
    return chunk_var;
}

void ccn_data::setChunk(unsigned long chunk)
{
    this->chunk_var = chunk;
}

double ccn_data::getD() const
{
    return d_var;
}

void ccn_data::setD(double d)
{
    this->d_var = d;
}

double ccn_data::getHops() const
{
    return hops_var;
}

void ccn_data::setHops(double hops)
{
    this->hops_var = hops;
}

unsigned long ccn_data::getRepository() const
{
    return repository_var;
}

void ccn_data::setRepository(unsigned long repository)
{
    this->repository_var = repository;
}

class ccn_dataDescriptor : public cClassDescriptor
{
  public:
    ccn_dataDescriptor();
    virtual ~ccn_dataDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(ccn_dataDescriptor);

ccn_dataDescriptor::ccn_dataDescriptor() : cClassDescriptor("ccn_data", "cMessage")
{
}

ccn_dataDescriptor::~ccn_dataDescriptor()
{
}

bool ccn_dataDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<ccn_data *>(obj)!=NULL;
}

const char *ccn_dataDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int ccn_dataDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int ccn_dataDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *ccn_dataDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "chunk",
        "d",
        "hops",
        "repository",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int ccn_dataDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='c' && strcmp(fieldName, "chunk")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "d")==0) return base+1;
    if (fieldName[0]=='h' && strcmp(fieldName, "hops")==0) return base+2;
    if (fieldName[0]=='r' && strcmp(fieldName, "repository")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *ccn_dataDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "unsigned long",
        "double",
        "double",
        "unsigned long",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *ccn_dataDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int ccn_dataDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    ccn_data *pp = (ccn_data *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string ccn_dataDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    ccn_data *pp = (ccn_data *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getChunk());
        case 1: return double2string(pp->getD());
        case 2: return double2string(pp->getHops());
        case 3: return ulong2string(pp->getRepository());
        default: return "";
    }
}

bool ccn_dataDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    ccn_data *pp = (ccn_data *)object; (void)pp;
    switch (field) {
        case 0: pp->setChunk(string2ulong(value)); return true;
        case 1: pp->setD(string2double(value)); return true;
        case 2: pp->setHops(string2double(value)); return true;
        case 3: pp->setRepository(string2ulong(value)); return true;
        default: return false;
    }
}

const char *ccn_dataDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
        NULL,
        NULL,
    };
    return (field>=0 && field<4) ? fieldStructNames[field] : NULL;
}

void *ccn_dataDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    ccn_data *pp = (ccn_data *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


