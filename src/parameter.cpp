/**
 * This file is part of Special K.
 *
 * Special K is free software : you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * as published by The Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Special K is distributed in the hope that it will be useful,
 *
 * But WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Special K.
 *
 *   If not, see <http://www.gnu.org/licenses/>.
 *
**/

#include <SpecialK/parameter.h>
#include <SpecialK/utility.h>
#include <SpecialK/ini.h>

// Read value from INI
bool
sk::iParameter::load (void)
{
  if (ini != nullptr)
  {
    const wchar_t*  sec_name =
      ini_section.c_str ();

    if (ini->contains_section (sec_name))
    {
      iSK_INISection& section =
        ini->get_section (sec_name);

      if (section.contains_key (ini_key.c_str ()))
      {
        std::wstring& new_value =
          section.get_value (ini_key.c_str ());

        if (! new_value.empty ())
        {
          set_value_str (new_value);

          return true;
        }
      }
    }
  }

  return false;
}

bool
sk::iParameter::store (void)
{
  bool ret = false;

  if (ini != nullptr)
  {
    iSK_INISection& section =
      ini->get_section (ini_section.c_str ());

    section.add_key_value ( ini_key.c_str (),
                            get_value_str ().c_str () );

    ret = true;
  }

  return ret;
}


std::wstring
sk::ParameterInt::get_value_str (void)
{
  wchar_t str [32] = { };
  _itow (value, str, 10);

  return
    str;
}

int
sk::ParameterInt::get_value (void)
{
  return value;
}

void
sk::ParameterInt::set_value (int val)
{
  value = val;
}


void
sk::ParameterInt::set_value_str (const wchar_t *str)
{
  value = _wtoi (str);
}

void
sk::ParameterInt::set_value_str (const std::wstring& str)
{
  set_value_str (str.c_str ());
}


void
sk::ParameterInt::store (int val)
{
  set_value      (val);
  iParameter::store ();
}

void
sk::ParameterInt::store_str (const wchar_t *str)
{
  set_value_str  (str);
  iParameter::store ();
}

void
sk::ParameterInt::store_str (const std::wstring& str)
{
  store_str (str.c_str ());
}

bool
sk::ParameterInt::load (int& ref)
{
  bool bRet =
    iParameter::load ();

  if (bRet)
    ref = get_value ();

  return bRet;
}


std::wstring
sk::ParameterInt64::get_value_str (void)
{
  wchar_t str [32] = { };
  _i64tow (value, str, 10);

  return
    str;
}

int64_t
sk::ParameterInt64::get_value (void)
{
  return value;
}

void
sk::ParameterInt64::set_value (int64_t val)
{
  value = val;
}


void
sk::ParameterInt64::set_value_str (const wchar_t* str)
{
  value = _wtoll (str);
}

void
sk::ParameterInt64::set_value_str (const std::wstring& str)
{
  set_value_str (str.c_str ());
}


void
sk::ParameterInt64::store (int64_t val)
{
  set_value      (val);
  iParameter::store ();
}

void
sk::ParameterInt64::store_str (const wchar_t *str)
{
  set_value_str  (str);
  iParameter::store ();
}

void
sk::ParameterInt64::store_str (const std::wstring& str)
{
  store_str (str.c_str ());
}

bool
sk::ParameterInt64::load (int64_t& ref)
{
  bool bRet =
    iParameter::load ();

  if (bRet)
    ref = get_value ();

  return bRet;
}


std::wstring
sk::ParameterBool::get_value_str (void)
{
#ifdef CAP_FIRST_LETTER
  switch (type) {
    case ZeroNonZero:
      return value  ?  L"1"    : L"0";
    case YesNo:
      return value  ?  L"Yes"  : L"No";
    case OnOff:
      return value  ?  L"On"   : L"Off";
    case TrueFalse:
    default:
      return value  ?  L"True" : L"False";
  }
#else
  // Traditional behavior, more soothing ont he eyes ;)
  switch (type) {
    case ZeroNonZero:
      return value  ?  L"1"    : L"0";
    case YesNo:
      return value  ?  L"yes"  : L"no";
    case OnOff:
      return value  ?  L"on"   : L"off";
    case TrueFalse:
    default:
      return value  ?  L"true" : L"false";
  }
#endif
}

bool
sk::ParameterBool::get_value (void)
{
  return value;
}

void
sk::ParameterBool::set_value (bool val)
{
  value = val;
}


void
sk::ParameterBool::set_value_str (const wchar_t *str)
{
  size_t len = wcslen (str);

  type = TrueFalse;

  switch (len)
  {
    case 1:
      type = ZeroNonZero;

      if (str [0] == L'1')
        value = true;
      break;

    case 2:
      if ( towlower (str [0]) == L'o' &&
           towlower (str [1]) == L'n' ) {
        type  = OnOff;
        value = true;
      } else if ( towlower (str [0]) == L'n' &&
                  towlower (str [1]) == L'o' ) {
        type  = YesNo;
        value = false;
      }
      break;

    case 3:
      if ( towlower (str [0]) == L'y' &&
           towlower (str [1]) == L'e' &&
           towlower (str [2]) == L's' ) {
        type  = YesNo;
        value = true;
      } else if ( towlower (str [0]) == L'o' &&
                  towlower (str [1]) == L'f' &&
                  towlower (str [2]) == L'f' ) {
        type  = OnOff;
        value = false;
      }
      break;

    case 4:
      if ( towlower (str [0]) == L't' &&
           towlower (str [1]) == L'r' &&
           towlower (str [2]) == L'u' &&
           towlower (str [3]) == L'e' )
        value = true;
      break;

    default:
      value = false;
      break;
  }
}

void
sk::ParameterBool::set_value_str (const std::wstring& str)
{
  size_t len = str.length ();

  type = TrueFalse;

  switch (len)
  {
    case 1:
      type = ZeroNonZero;

      if (str [0] == L'1')
        value = true;
      break;

    case 2:
      if ( towlower (str [0]) == L'o' &&
           towlower (str [1]) == L'n' ) {
        type  = OnOff;
        value = true;
      } else if ( towlower (str [0]) == L'n' &&
                  towlower (str [1]) == L'o' ) {
        type  = YesNo;
        value = false;
      }
      break;

    case 3:
      if ( towlower (str [0]) == L'y' &&
           towlower (str [1]) == L'e' &&
           towlower (str [2]) == L's' ) {
        type  = YesNo;
        value = true;
      } else if ( towlower (str [0]) == L'o' &&
                  towlower (str [1]) == L'f' &&
                  towlower (str [2]) == L'f' ) {
        type  = OnOff;
        value = false;
      }
      break;

    case 4:
      if ( towlower (str [0]) == L't' &&
           towlower (str [1]) == L'r' &&
           towlower (str [2]) == L'u' &&
           towlower (str [3]) == L'e' )
        value = true;
      break;

    default:
      value = false;
      break;
  }
}

void
sk::ParameterBool::store (bool val)
{
  set_value      (val);
  iParameter::store ();
}

void
sk::ParameterBool::store_str (const wchar_t *str)
{
  set_value_str  (str);
  iParameter::store ();
}

void
sk::ParameterBool::store_str (const std::wstring& str)
{
  store_str (str.c_str ());
}

bool
sk::ParameterBool::load (bool& ref)
{
  bool bRet =
    iParameter::load ();

  if (bRet)
    ref = get_value ();

  return bRet;
}



std::wstring
sk::ParameterFloat::get_value_str (void)
{
  wchar_t val_str [325] = { };
  swprintf (val_str, L"%f", value);

  SK_RemoveTrailingDecimalZeros (val_str);

  return
    val_str;
}

float
sk::ParameterFloat::get_value (void)
{
  return value;
}

void
sk::ParameterFloat::set_value (float val)
{
  value = val;
}

void
sk::ParameterFloat::set_value_str (const wchar_t *str)
{
  value =
    static_cast <float> (
      wcstod (str, nullptr)
    );
}

void
sk::ParameterFloat::set_value_str (const std::wstring& str)
{
  set_value_str (str.c_str ());
}


void
sk::ParameterFloat::store (float val)
{
  set_value      (val);
  iParameter::store ();
}

void
sk::ParameterFloat::store_str (const wchar_t *str)
{
  set_value_str  (str);
  iParameter::store ();
}

void
sk::ParameterFloat::store_str (const std::wstring& str)
{
  store_str (str.c_str ());
}

bool
sk::ParameterFloat::load (float& ref)
{
  bool bRet =
    iParameter::load ();

  if (bRet)
    ref = get_value ();

  return bRet;
}


std::wstring
sk::ParameterStringW::get_value_str (void)
{
  return value;
}

std::wstring
sk::ParameterStringW::get_value (void)
{
  return value;
}

std::wstring&
sk::ParameterStringW::get_value_ref (void)
{
  return value;
}

void
sk::ParameterStringW::set_value (const wchar_t* val)
{
  value = val;
}

void
sk::ParameterStringW::set_value (std::wstring val)
{
  value = val;
}


void
sk::ParameterStringW::set_value_str (const wchar_t *str)
{
  value = str;
}

void
sk::ParameterStringW::set_value_str (const std::wstring& str)
{
  value = str;
}

void
sk::ParameterStringW::store (std::wstring val)
{
  set_value      (val);
  iParameter::store ();
}

void
sk::ParameterStringW::store_str (const wchar_t *str)
{
  set_value_str  (str);
  iParameter::store ();
}

void
sk::ParameterStringW::store_str (const std::wstring& str)
{
  store_str (str.c_str ());
}

bool
sk::ParameterStringW::load (std::wstring& ref)
{
  bool bRet =
    iParameter::load ();

  if (bRet)
    ref = get_value ();

  return bRet;
}



std::wstring
sk::ParameterVec2f::get_value_str (void)
{
  wchar_t x_str [325] = { };
  wchar_t y_str [325] = { };

  swprintf (x_str, L"%f", value.x);
  swprintf (y_str, L"%f", value.y);

  SK_RemoveTrailingDecimalZeros (x_str);
  SK_RemoveTrailingDecimalZeros (y_str);

  return SK_FormatStringW (L"(%s,%s)", x_str, y_str);
}

ImVec2
sk::ParameterVec2f::get_value (void)
{
  return value;
}

void
sk::ParameterVec2f::set_value (ImVec2 val)
{
  value = val;
}

void
sk::ParameterVec2f::set_value_str (const wchar_t *str)
{
  swscanf (str, L"(%f,%f)", &value.x, &value.y);
}

void
sk::ParameterVec2f::set_value_str (const std::wstring& str)
{
  set_value_str (str.c_str ());
}

void
sk::ParameterVec2f::store (ImVec2 val)
{
  set_value      (val);
  iParameter::store ();
}

void
sk::ParameterVec2f::store_str (const wchar_t *str)
{
  store_str      (str);
  set_value_str  (str);
  iParameter::store ();
}

void
sk::ParameterVec2f::store_str (const std::wstring& str)
{
  store_str (str.c_str ());
}

bool
sk::ParameterVec2f::load (ImVec2& ref)
{
  const bool bRet =
    iParameter::load ();

  if (bRet)
    ref = get_value ();

  return bRet;
}


template <>
sk::iParameter*
sk::ParameterFactory::create_parameter <int> (const wchar_t* name)
{
  UNREFERENCED_PARAMETER (name);

  std::lock_guard <std::mutex> _scope_lock (lock);

  params.emplace_back (
    std::make_unique <ParameterInt> ()
  );

  return params.back ().get ();
}

#if 0
template<>
sk::iParameter*
sk::ParameterFactory::create_parameter (const wchar_t* name)
{
  UNREFERENCED_PARAMETER (name);

  return nullptr;
}
#endif

template <>
sk::iParameter*
sk::ParameterFactory::create_parameter <int64_t> (const wchar_t* name)
{
  UNREFERENCED_PARAMETER (name);

  std::lock_guard <std::mutex> _scope_lock (lock);

  params.emplace_back (
    std::make_unique <ParameterInt64> ()
  );

  return params.back ().get ();
}

template <>
sk::iParameter*
sk::ParameterFactory::create_parameter <bool> (const wchar_t* name)
{
  UNREFERENCED_PARAMETER (name);

  std::lock_guard <std::mutex> _scope_lock (lock);

  params.emplace_back (
    std::make_unique <ParameterBool> ()
  );

  return params.back ().get ();
}

template <>
sk::iParameter*
sk::ParameterFactory::create_parameter <float> (const wchar_t* name)
{
  UNREFERENCED_PARAMETER (name);

  std::lock_guard <std::mutex> _scope_lock (lock);

  params.emplace_back (
    std::make_unique <ParameterFloat> ()
  );

  return params.back ().get ();
}

template <>
sk::iParameter*
sk::ParameterFactory::create_parameter <std::wstring> (const wchar_t* name)
{
  UNREFERENCED_PARAMETER (name);

  std::lock_guard <std::mutex> _scope_lock (lock);

  params.emplace_back (
    std::make_unique <ParameterStringW> ()
  );

  return params.back ().get ();
}

template <>
sk::iParameter*
sk::ParameterFactory::create_parameter <ImVec2> (const wchar_t* name)
{
  UNREFERENCED_PARAMETER (name);

  std::lock_guard <std::mutex> _scope_lock (lock);

  params.emplace_back (
    std::make_unique <ParameterVec2f> ()
  );

  return params.back ().get ();
}


#if 0
bool
STDMETHODCALLTYPE
iSK_Parameter::load (void)
{
  if (ini != nullptr) {
    iSK_INISection& section = ini->get_section (ini_section);

    if (section.contains_key (ini_key)) {
      set_value_str (section.get_value (ini_key));
      return true;
    }
  }

  return false;
}

bool
STDMETHODCALLTYPE
iSK_Parameter::store (void)
{
  bool ret = false;

  if (ini != nullptr) {
    iSK_INISection& section = ini->get_section (ini_section);

    // If this operation actually creates a section, we need to make sure
    //   that section has a name!
    section.set_name (ini_section);

    if (section.contains_key (ini_key)) {
      section.get_value (ini_key) = get_value_str ();
      ret = true;
    }

    // Add this key/value if it doesn't already exist.
    else {
      section.add_key_value (ini_key, get_value_str ().c_str ());
      ret = true;// +1;
    }
  }

  return ret;
}
#endif

#if 0
void
STDMETHODCALLTYPE
iSK_ParameterBase::register_to_ini ( iSK_INI      *file,
                                     std::wstring  section,
                                     std::wstring  key )
{
  ini         = file;
  ini_section = section;
  ini_key     = key;
}
#endif