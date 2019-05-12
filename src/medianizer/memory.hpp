/// Memory utilities.
///
/// @file

#include <type_traits>

#include <boost/core/noncopyable.hpp>

//==========================================================================
namespace medianizer {
//==========================================================================

/// Generic RAII wrapper around raw C pointers.
/// Like std::unique_ptr but not limited to a memory allocation.
template<typename _T, auto _Deleter>
class CPtr final : boost::noncopyable
{
public:
  CPtr() noexcept = default;

  CPtr(_T* const ptr) noexcept
    : m_ptr{ptr}
  {}

  CPtr(CPtr&& other) noexcept
  {
    del();
    m_ptr = other.m_ptr;
    other.m_ptr = nullptr;
  }

  CPtr& operator=(CPtr&& other) noexcept
  {
    del();
    m_ptr = other.m_ptr;
    other.m_ptr = nullptr;
  }

  ~CPtr()
  {
    del();
  }

  auto& operator*() const noexcept
  {
    return *m_ptr;
  }

  auto operator->() const noexcept
  {
    return m_ptr;
  }

  auto get() const noexcept
  {
    return m_ptr;
  }

private:
  void del()
  {
    if (m_ptr != nullptr)
    {
      _Deleter(m_ptr);
      m_ptr = nullptr;
    }
  }

  _T* m_ptr{};
};

//==========================================================================

/// AV* objects RAII wrapper.
template<typename _T, auto _Deleter>
class AVPtr final
{
  public:
    AVPtr() noexcept = default;

    AVPtr(_T* const ptr) noexcept
      : m_ptr{ptr}
    {}

  auto& operator*() const noexcept
  {
    return *m_ptr;
  }

  auto operator->() const noexcept
  {
    return m_ptr.get();
  }

  auto get() const noexcept
  {
    return m_ptr.get();
  }

  private:
    static void deleter(_T* ptr)
    {
      _Deleter(&ptr);
    }

    CPtr<_T, deleter> m_ptr{};
};

//==========================================================================
} // namespace medianizer
