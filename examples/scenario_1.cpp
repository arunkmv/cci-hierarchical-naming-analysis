#include <cci_configuration>
#include <iostream>
#include <systemc>

const char *A_MOD_NAME = "A";
const char *B_MOD_NAME = "B";
const char *C_PARAM_NAME = "B.c";
const int C_PARAM_DEFAULT_VALUE = 10;

class B : public sc_core::sc_module {
public:
  B(const sc_core::sc_module_name &mod_name) : sc_core::sc_module(mod_name) {
    std::cout << "Instantiated module: " << name() << std::endl;
  }
};

class A : public sc_core::sc_module {
public:
  A(const sc_core::sc_module_name &mod_name)
      : sc_core::sc_module(mod_name), m_b(B_MOD_NAME),
        m_c_param(C_PARAM_NAME, C_PARAM_DEFAULT_VALUE,
                  "Param named B.c within mod A", cci::CCI_RELATIVE_NAME) {
    std::cout << "Instantiated module: " << name() << std::endl;
  }

  void SetParamValue(const int val) { m_c_param = val; }

  int GetParamValue() { return m_c_param.get_value(); }

private:
  cci::cci_param<int, cci::CCI_MUTABLE_PARAM>
      m_c_param; /**< Mutable param named B.c */
  B m_b;         /**< Nested module B */
};

int sc_main(int argc, char *argv[]) {
  std::cout << "====================" << std::endl;
  std::cout << "     Scenario 1     " << std::endl;
  std::cout << "====================" << std::endl;

  // Derive parameter name for relative naming
  std::string par_name =
      std::string(A_MOD_NAME) + "." + std::string(C_PARAM_NAME);

  // Register CCI configuration broker
  cci::cci_broker_handle m_broker = cci::cci_register_broker(
      new cci_utils::broker("Global Broker"));

  m_broker.set_preset_cci_value(par_name, cci::cci_value(20));

  // Instantiate module hierarchy
  A m_a(A_MOD_NAME);

  // Verify that parameter is registered in broker
  cci::cci_param_typed_handle<int> par_handle =
      m_broker.get_param_handle<int>(par_name);
  if (par_handle.is_valid()) {
    std::cout << "Parameter " << par_handle.name() << " is registered in broker"
              << std::endl;
  } else {
    std::cout << "Parameter " << par_name << " is not registered in broker"
              << std::endl;
    return 1;
  }

  std::cout << "Value of parameter from Module A: " << m_a.GetParamValue()
            << std::endl;
  std::cout << "Value of parameter from Broker: " << par_handle.get_value()
            << std::endl;

  int new_par_value = 30;
  std::cout << "Setting value of parameter through Module A to "
            << new_par_value << std::endl;
  m_a.SetParamValue(new_par_value);

  std::cout << "Value of parameter from Module A: " << m_a.GetParamValue()
            << std::endl;
  std::cout << "Value of parameter from Broker: " << par_handle.get_value()
            << std::endl;

  new_par_value = 40;
  std::cout << "Setting value of parameter through Broker to " << new_par_value
            << std::endl;
  par_handle.set_value(new_par_value);
  std::cout << "Value of parameter from Module A: " << m_a.GetParamValue()
            << std::endl;
  std::cout << "Value of parameter from Broker: " << par_handle.get_value()
            << std::endl;

  return 0;
}
