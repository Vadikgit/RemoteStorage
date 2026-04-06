#pragma once

#include <vector>
#include <cstdint>
#include <functional>
#include "./btree/btree.h"
#include "./binserializer/entity.h"

class PointBTWrapper : public BTProcessable
{
public:
    entity::point point_;

    virtual void fromBytes(const std::vector<uint8_t> &ibuf, size_t firstBytePos) override;
    virtual void toBytes(std::vector<uint8_t> &obuf, size_t firstBytePos) override;
    virtual bool less(const BTProcessable &w2) const override;
    virtual size_t sizeInBytes() override;
    virtual PointBTWrapper *createNew() override;

    PointBTWrapper() : BTProcessable() {};
    PointBTWrapper(uint64_t xx, uint64_t yy, uint64_t zz) : BTProcessable()
    {
        point_.set_x(xx);
        point_.set_y(yy);
        point_.set_z(zz);
    };

    virtual PointBTWrapper &operator=(const BTProcessable &other) override;
    virtual PointBTWrapper &operator=(BTProcessable &&other) override;
};

PointBTWrapper &PointBTWrapper::operator=(const BTProcessable &other)
{
    if (this != &other)
    {
        point_.set_x(reinterpret_cast<const PointBTWrapper &>(other).point_.get_x());
        point_.set_y(reinterpret_cast<const PointBTWrapper &>(other).point_.get_y());
        point_.set_z(reinterpret_cast<const PointBTWrapper &>(other).point_.get_z());
    }

    return *this;
}

PointBTWrapper &PointBTWrapper::operator=(BTProcessable &&other)
{
    if (this != &other)
    {
        point_.set_x(reinterpret_cast<PointBTWrapper &&>(other).point_.get_x());
        point_.set_y(reinterpret_cast<PointBTWrapper &&>(other).point_.get_y());
        point_.set_z(reinterpret_cast<PointBTWrapper &&>(other).point_.get_z());
    }

    return *this;
}

void PointBTWrapper::fromBytes(const std::vector<uint8_t> &ibuf, size_t firstBytePos)
{
    point_.set_x(*(reinterpret_cast<const int32_t *>(&ibuf[firstBytePos])));
    firstBytePos += sizeof(int32_t);

    point_.set_y(*(reinterpret_cast<const int32_t *>(&ibuf[firstBytePos])));
    firstBytePos += sizeof(int32_t);

    point_.set_z(*(reinterpret_cast<const int32_t *>(&ibuf[firstBytePos])));
    firstBytePos += sizeof(int32_t);
}

void PointBTWrapper::toBytes(std::vector<uint8_t> &obuf, size_t firstBytePos)
{
    *(reinterpret_cast<int32_t *>(&obuf[firstBytePos])) = point_.get_x();
    firstBytePos += sizeof(int32_t);

    *(reinterpret_cast<int32_t *>(&obuf[firstBytePos])) = point_.get_y();
    firstBytePos += sizeof(int32_t);

    *(reinterpret_cast<int32_t *>(&obuf[firstBytePos])) = point_.get_z();
    firstBytePos += sizeof(int32_t);
}

bool PointBTWrapper::less(const BTProcessable &p2) const
{
    auto x = point_.get_x();
    auto otherX = dynamic_cast<const PointBTWrapper &>(p2).point_.get_x();

    auto y = point_.get_y();
    auto otherY = dynamic_cast<const PointBTWrapper &>(p2).point_.get_y();

    auto z = point_.get_z();
    auto otherZ = dynamic_cast<const PointBTWrapper &>(p2).point_.get_z();

    if (x != otherX)
        return x < otherX;

    if (y != otherY)
        return y < otherY;

    return z < otherZ;
}

size_t PointBTWrapper::sizeInBytes()
{
    return sizeof(point_.get_x()) + sizeof(point_.get_y()) + sizeof(point_.get_z());
}

PointBTWrapper *PointBTWrapper::createNew()
{
    return new PointBTWrapper();
}

// bool operator<(entity::point &ep1, entity::point &ep2)
// {
// 	if (ep1.get_x() != ep2.get_x())
// 		return ep1.get_x() < ep2.get_x();

// 	if (ep1.get_y() != ep2.get_y())
// 		return ep1.get_y() < ep2.get_y();

// 	if (ep1.get_z() != ep2.get_z())
// 		return ep1.get_z() < ep2.get_z();

// 	return false;
// }

// bool operator==(entity::point &ep1, entity::point &ep2)
// {
// 	if (ep1.get_x() == ep2.get_x() && ep1.get_y() == ep2.get_y() && ep1.get_z() == ep2.get_z())
// 		return true;

// 	return false;
// }

// bool operator!=(entity::point &ep1, entity::point &ep2)
// {
// 	return !(ep1 == ep2);
// }

// bool operator>(entity::point &ep1, entity::point &ep2)
// {
// 	return (ep1 != ep2 && !(ep1 < ep2));
// }

// bool operator>=(entity::point &ep1, entity::point &ep2)
// {
// 	return (ep1 == ep2 || ep1 > ep2);
// }

// bool operator<=(entity::point &ep1, entity::point &ep2)
// {
// 	return (ep1 == ep2 || ep1 < ep2);
// }
