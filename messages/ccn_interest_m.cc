//
// Generated file, do not edit! Created by opp_msgc 4.2 from messages/ccn_interest.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "ccn_interest_m.h"

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




ccn_interest_Base::ccn_interest_Base(const char *name, int kind) : cMessage(name,kind)
{
    this->chunk_var = 0;
    this->hops_var = 0;
    this->closest_var = false;
    this->repository_var = 0;
}

ccn_interest_Base::ccn_interest_Base(const ccn_interest_Base& other) : cMessage(other)
{
    copy(other);
}

ccn_interest_Base::~ccn_interest_Base()
{
}

ccn_interest_Base& ccn_interest_Base::operator=(const ccn_interest_Base& other)
{
    if (this==&other) return *this;
    cMessage::operator=(other);
    copy(other);
    return *this;
}

void ccn_interest_Base::copy(const ccn_interest_Base& other)
{
    this->chunk_var = other.chunk_var;
    this->hops_var = other.hops_var;
    this->closest_var = other.closest_var;
    this->repository_var = other.repository_var;
}

void ccn_interest_Base::parsimPack(cCommBuffer *b)
{
    cMessage::parsimPack(b);
    doPacking(b,this->chunk_var);
    doPacking(b,this->hops_var);
    doPacking(b,this->closest_var);
    // field path is abstract -- please do packing in customized class
    doPacking(b,this->repository_var);
}

void ccn_interest_Base::parsimUnpack(cCommBuffer *b)
{
    cMessage::parsimUnpack(b);
    doUnpacking(b,this->chunk_var);
    doUnpacking(b,this->hops_var);
    doUnpacking(b,this->closest_var);
    // field path is abstract -- please do unpacking in customized class
    doUnpacking(b,this->repository_var);
}

unsigned long ccn_interest_Base::getChunk() const
{
    return chunk_var;
}

void ccn_interest_Base::setChunk(unsigned long chunk)
{
    this->chunk_var = chunk;
}

double ccn_interest_Base::getHops() const
{
    return hops_var;
}

void ccn_interest_Base::setHops(double hops)
{
    this->hops_var = hops;
}

bool ccn_interest_Base::getClosest() const
{
    return closest_var;
}

void ccn_interest_Base::setClosest(bool closest)
{
    this->closest_var = closest;
}

unsigned long ccn_interest_Base::getRepository() const
{
    return repository_var;
}

void ccn_interest_Base::setRepository(unsigned long repository)
{
    this->repository_var = repository;
}

class ccn_interestDescriptor : public cClassDescriptor
{
  public:
    ccn_interestDescriptor();
    virtual ~ccn_interestDescriptor();

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

Register_ClassDescriptor(ccn_interestDescriptor);

ccn_interestDescriptor::ccn_interestDescriptor() : cClassDescriptor("ccn_interest", "cMessage")
{
}

ccn_interestDescriptor::~ccn_interestDescriptor()
{
}

bool ccn_interestDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<ccn_interest_Base *>(obj)!=NULL;
}

const char *ccn_interestDescriptor::getProperty(const char *propertyname) const
{
    if (!strcmp(propertyname,"customize")) return "true";
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int ccn_interestDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount(object) : 5;
}

unsigned int ccn_interestDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<5) ? fieldTypeFlags[field] : 0;
}

const char *ccn_interestDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "chunk",
        "hops",
        "closest",
        "path",
        "repository",
    };
    return (field>=0 && field<5) ? fieldNames[field] : NULL;
}

int ccn_interestDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='c' && strcmp(fieldName, "chunk")==0) return base+0;
    if (fieldName[0]=='h' && strcmp(fieldName, "hops")==0) return base+1;
    if (fieldName[0]=='c' && strcmp(fieldName, "closest")==0) return base+2;
    if (fieldName[0]=='p' && strcmp(fieldName, "path")==0) return base+3;
    if (fieldName[0]=='r' && strcmp(fieldName, "repository")==0) return base+4;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *ccn_interestDescriptor::getFieldTypeString(void *object, int field) const
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
        "bool",
        "int",
        "unsigned long",
    };
    return (field>=0 && field<5) ? fieldTypeStrings[field] : NULL;
}

const char *ccn_interestDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int ccn_interestDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    ccn_interest_Base *pp = (ccn_interest_Base *)object; (void)pp;
    switch (field) {
        case 3: return pp->getPathArraySize();
        default: return 0;
    }
}

std::string ccn_interestDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    ccn_interest_Base *pp = (ccn_interest_Base *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getChunk());
        case 1: return double2string(pp->getHops());
        case 2: return bool2string(pp->getClosest());
        case 3: return long2string(pp->getPath(i));
        case 4: return ulong2string(pp->getRepository());
        default: return "";
    }
}

bool ccn_interestDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    ccn_interest_Base *pp = (ccn_interest_Base *)object; (void)pp;
    switch (field) {
        case 0: pp->setChunk(string2ulong(value)); return true;
        case 1: pp->setHops(string2double(value)); return true;
        case 2: pp->setClosest(string2bool(value)); return true;
        case 3: pp->setPath(i,string2long(value)); return true;
        case 4: pp->setRepository(string2ulong(value)); return true;
        default: return false;
    }
}

const char *ccn_interestDescriptor::getFieldStructName(void *object, int field) const
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
        NULL,
    };
    return (field>=0 && field<5) ? fieldStructNames[field] : NULL;
}

void *ccn_interestDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    ccn_interest_Base *pp = (ccn_interest_Base *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


