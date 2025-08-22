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
    std::cout << "Instantiated parameter: " << m_c_param.name()
              << " with default value: " << C_PARAM_DEFAULT_VALUE << std::endl;
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
  cci_utils::broker* global_broker = new cci_utils::broker("Global Broker");
  cci::cci_register_broker(global_broker);
  cci::cci_originator m_orig = cci::cci_originator("sc_main");

  // Get broker handle
  cci::cci_broker_handle m_broker = cci::cci_get_global_broker(m_orig);
  
  int next_par_val = 20;
  std::cout << "Setting preset value through broker to: " << next_par_val
            << std::endl;
  m_broker.set_preset_cci_value(par_name, cci::cci_value(next_par_val));

  std::cout << m_broker.name() << std::endl;

  // Instantiate module hierarchy
  A m_a(A_MOD_NAME);

  // Verify that parameter is registered in broker
  cci::cci_param_typed_handle<int> par_handle =
      m_broker.get_param_handle<int>(par_name);
  sc_assert(par_handle.is_valid() && "Parameter is not registered!");

  // Check if preset value is used
  std::cout << "Value of parameter from Module A: " << m_a.GetParamValue()
            << std::endl;
  std::cout << "Value of parameter from Broker: " << par_handle.get_value()
            << std::endl;
  std::cout << "Is preset value?: " << m_broker.has_preset_value(par_name) << std::endl;

  next_par_val = 30;
  std::cout << "Setting value of parameter through Module A to: "
            << next_par_val << std::endl;
  m_a.SetParamValue(next_par_val);

  std::cout << "Value of parameter from Module A: " << m_a.GetParamValue()
            << std::endl;
  std::cout << "Value of parameter from Broker: " << par_handle.get_value()
            << std::endl;

  next_par_val = 40;
  std::cout << "Setting value of parameter through Broker to " << next_par_val
            << std::endl;
  par_handle.set_value(next_par_val);
  std::cout << "Value of parameter from Module A: " << m_a.GetParamValue()
            << std::endl;
  std::cout << "Value of parameter from Broker: " << par_handle.get_value()
            << std::endl;

  return 0;
}
