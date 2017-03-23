#ifndef __CONTROLLER_PROPERTYLIST_H__
#define __CONTROLLER_PROPERTYLIST_H__

#include <Nebulae/Common/Common.h>

namespace Game {

class PropertyArray
///
/// A simple modifiable list of properties.
///
{
private:
  std::map<std::string,boost::any> m_properties;  ///< A list of properties.

  public:
    /** \name Structors */ ///@{
    PropertyArray();
    ~PropertyArray();
    //@}

    /** \name Mutators */ ///@{
    bool Load( Nebulae::File& stream );
    void Load( const Json::Value& json );
    void Put( const std::string& name, const boost::any& value );
    //@}

    /** \name Accessors */ ///@{
    bool Find( const std::string& name, boost::any& out ) const;
    //@}

};

}

#endif // __CONTROLLER_PROPERTYLIST_H__