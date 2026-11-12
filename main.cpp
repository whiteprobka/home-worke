#include <cstddef>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <utility>

class Ints
{
public:
  Ints();
  Ints(size_t size, int value);
  Ints(const Ints& rhs);
  Ints& operator=(const Ints& rhs);
  ~Ints();

  size_t getSize() const noexcept;
  void set(size_t id, int i) noexcept;
  int get(size_t id) const noexcept;
  void setSafe(size_t id, int i);
  int getSafe(size_t id) const;

  Ints append(int i) const;
  Ints append(const Ints& b) const;

  static Ints unitializatedInts(size_t size);

private:
  void resize();
  size_t capacity_;
  size_t size_;
  int* data_;
};

Ints::Ints() :
  capacity_(0),
  size_(0),
  data_(nullptr)
{
}

Ints::Ints(size_t size, int value) :
  capacity_(size),
  size_(size),
  data_(new int[size])
{
  for (size_t i = 0; i < size_; ++i)
  {
    data_[i] = value;
  }
}

Ints Ints::unitializatedInts(size_t size)
{
  Ints temp;
  temp.data_ = new int[size];
  temp.capacity_ = size;
  temp.size_ = size;
  return temp;
}

Ints::Ints(const Ints& rhs) :
  capacity_(rhs.size_),
  size_(rhs.size_),
  data_(new int[rhs.size_])
{
  for (size_t i = 0; i < size_; ++i)
  {
    data_[i] = rhs.data_[i];
  }
}

Ints& Ints::operator=(const Ints& rhs)
{
  if (this == &rhs)
  {
    return *this;
  }

  Ints temp(rhs);
  std::swap(capacity_, temp.capacity_);
  std::swap(size_, temp.size_);
  std::swap(data_, temp.data_);

  return *this;
}

Ints::~Ints()
{
  delete[] data_;
}

size_t Ints::getSize() const noexcept
{
  return size_;
}

void Ints::set(size_t id, int i) noexcept
{
  data_[id] = i;
}

int Ints::get(size_t id) const noexcept
{
  return data_[id];
}

void Ints::setSafe(size_t id, int i)
{
  if (id >= size_)
  {
    throw std::logic_error("Invalid index");
  }

  set(id, i);
}

int Ints::getSafe(size_t id) const
{
  if (id >= size_)
  {
    throw std::logic_error("Invalid index");
  }

  return get(id);
}

void Ints::resize()
{
  size_t newCapacity = (capacity_ == 0) ? 1 : capacity_ * 2;
  int* newData = new int[newCapacity];

  for (size_t i = 0; i < size_; ++i)
  {
    newData[i] = data_[i];
  }

  delete[] data_;
  data_ = newData;
  capacity_ = newCapacity;
}

Ints Ints::append(int i) const
{
  Ints result(*this);

  if (result.capacity_ < result.size_ + 1)
  {
    result.resize();
  }

  result.data_[result.size_] = i;
  ++result.size_;

  return result;
}

Ints Ints::append(const Ints& b) const
{
  Ints result = unitializatedInts(size_ + b.size_);

  for (size_t i = 0; i < size_; ++i)
  {
    result.data_[i] = data_[i];
  }
  for (size_t i = 0; i < b.size_; ++i)
  {
    result.data_[size_ + i] = b.data_[i];
  }

  return result;
}

struct p_t
{
  int x;
  int y;
};

class Points
{
public:
  size_t getSize() const noexcept;
  void set(size_t id, p_t p);
  p_t get(size_t id) const;
  Points append(p_t p) const;

private:
  Ints data_;
};

size_t Points::getSize() const noexcept
{
  return data_.getSize() / 2;
}

void Points::set(size_t id, p_t p)
{
  data_.setSafe(2 * id, p.x);
  data_.setSafe(2 * id + 1, p.y);
}

p_t Points::get(size_t id) const
{
  return { data_.getSafe(2 * id), data_.getSafe(2 * id + 1) };
}

Points Points::append(p_t p) const
{
  Points result(*this);
  result.data_ = data_.append(p.x).append(p.y);
  return result;
}

class Plot
{
public:
  void set(int x, int y);
  int calc(int x) const;
  int solve(int y) const;
  Points append(p_t p) const;

private:
  Ints data_;
};

void Plot::set(int x, int y)
{
  for (size_t i = 0; i < data_.getSize(); i += 2)
  {
    if (data_.get(i) == x)
    {
      data_.set(i + 1, y);
      return;
    }
  }

  data_ = data_.append(x).append(y);
}

int Plot::calc(int x) const
{
  for (size_t i = 0; i < data_.getSize(); i += 2)
  {
    if (data_.get(i) == x)
    {
      return data_.get(i + 1);
    }
  }

  throw std::logic_error("No such x");
}

int Plot::solve(int y) const
{
  for (size_t i = 0; i < data_.getSize(); i += 2)
  {
    if (data_.get(i + 1) == y)
    {
      return data_.get(i);
    }
  }

  throw std::logic_error("No such y");
}

Points Plot::append(p_t p) const
{
  Points result;
  for (size_t i = 0; i < data_.getSize(); i += 2)
  {
    result = result.append({ data_.get(i), data_.get(i + 1) });
  }

  return result.append(p);
}

class Mtx
{
public:
  Mtx();
  Mtx(size_t m, size_t n);
  ~Mtx() = default;

  int get(size_t i, size_t j) const;
  void set(size_t i, size_t j, int value);

  Mtx sum(const Mtx& oth) const;
  Mtx mul(const Mtx& oth) const;
  Mtx transpose() const;
  int det() const;
  Mtx add_row(int init) const;

  Ints getCol(size_t id) const;
  Ints getRow(size_t id) const;

private:
  Mtx minor(size_t row, size_t col) const;
  size_t m_;
  size_t n_;
  Ints data_;
};

Mtx::Mtx() :
  m_(0),
  n_(0),
  data_()
{
}

Mtx::Mtx(size_t m, size_t n) :
  m_(m),
  n_(n),
  data_()
{
  if (n != 0 && m > std::numeric_limits< size_t >::max() / n)
  {
    throw std::logic_error("Overflow");
  }

  data_ = Ints(m * n, 0);
}

int Mtx::get(size_t i, size_t j) const
{
  return data_.getSafe(i * n_ + j);
}

void Mtx::set(size_t i, size_t j, int value)
{
  data_.setSafe(i * n_ + j, value);
}

Mtx Mtx::sum(const Mtx& oth) const
{
  if (m_ != oth.m_ || n_ != oth.n_)
  {
    throw std::logic_error("Different sizes");
  }

  Mtx res(m_, n_);
  for (size_t i = 0; i < data_.getSize(); ++i)
  {
    int a = data_.get(i);
    int b = oth.data_.get(i);

    if ((b > 0 && a > std::numeric_limits< int >::max() - b) ||
      (b < 0 && a < std::numeric_limits< int >::min() - b))
    {
      throw std::logic_error("Overflow");
    }

    res.data_.set(i, a + b);
  }

  return res;
}

Ints Mtx::getCol(size_t id) const
{
  Ints res(m_, 0);
  for (size_t i = 0; i < m_; ++i)
  {
    res.set(i, data_.get(i * n_ + id));
  }

  return res;
}

Ints Mtx::getRow(size_t id) const
{
  Ints res(n_, 0);
  for (size_t i = 0; i < n_; ++i)
  {
    res.set(i, data_.get(id * n_ + i));
  }

  return res;
}

Mtx Mtx::mul(const Mtx& oth) const
{
  if (n_ != oth.m_)
  {
    throw std::logic_error("Wrong sizes");
  }

  Mtx res(m_, oth.n_);
  for (size_t i = 0; i < m_; ++i)
  {
    Ints row = getRow(i);
    for (size_t j = 0; j < oth.n_; ++j)
    {
      Ints col = oth.getCol(j);

      int sm = 0;
      for (size_t z = 0; z < n_; ++z)
      {
        sm += row.get(z) * col.get(z);
      }

      res.data_.set(i * oth.n_ + j, sm);
    }
  }

  return res;
}

Mtx Mtx::transpose() const
{
  Mtx res(n_, m_);
  for (size_t i = 0; i < m_; ++i)
  {
    for (size_t j = 0; j < n_; ++j)
    {
      res.set(j, i, get(i, j));
    }
  }

  return res;
}

Mtx Mtx::minor(size_t row, size_t col) const
{
  Mtx res(m_ - 1, n_ - 1);
  size_t ri = 0;
  for (size_t i = 0; i < m_; ++i)
  {
    if (i == row)
    {
      continue;
    }

    size_t rj = 0;
    for (size_t j = 0; j < n_; ++j)
    {
      if (j == col)
      {
        continue;
      }

      res.set(ri, rj, get(i, j));
      ++rj;
    }
    ++ri;
  }

  return res;
}

int Mtx::det() const
{
  if (m_ != n_ || m_ == 0)
  {
    throw std::logic_error("Matrix must be square");
  }

  if (m_ == 1)
  {
    return get(0, 0);
  }

  int result = 0;
  int sign = 1;
  for (size_t j = 0; j < n_; ++j)
  {
    result += sign * get(0, j) * minor(0, j).det();
    sign = -sign;
  }

  return result;
}

Mtx Mtx::add_row(int init) const
{
  Mtx res(m_ + 1, n_);
  for (size_t i = 0; i < m_; ++i)
  {
    for (size_t j = 0; j < n_; ++j)
    {
      res.set(i, j, get(i, j));
    }
  }

  for (size_t j = 0; j < n_; ++j)
  {
    res.set(m_, j, init);
  }

  return res;
}

int main()
{
  Ints a(3, 67);
  Ints b(2, 69);
  Ints c = a.append(b).append(a).append(b).append(a);
  Points pts;
  Plot plot;
  Mtx x(2, 2);
}