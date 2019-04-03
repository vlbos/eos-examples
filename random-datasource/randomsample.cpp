#define ORACLIZE_NETWORK_NAME "eosio_testnet_jungle"

#include "oraclize/eos_api.hpp"

using namespace eosio;


class randomsample : public eosio::contract
{
  public:
    using contract::contract;

    randomsample(name receiver, name code, datastream<const char*> ds) : contract(receiver, code, ds) {}

    [[eosio::action]]
    void getrandnum()
    {
        print("Sending query to Oraclize...");
        uint8_t N = 1; // Possible outputs: [0-255]
        uint32_t delay = 10;
        oraclize_newRandomDSQuery(delay, N);
    }
    
    [[eosio::action]]
    void callback(capi_checksum256 queryId, std::vector<unsigned char> result, std::vector<unsigned char> proof)
    {
        require_auth(oraclize_cbAddress());

        if (oraclize_randomDS_proofVerify(queryId, result, proof, _self) != 0)
        {
            // The proof verification has failed, manage this use case...
        }
        else
        {
            uint8_t result_int = 0;
            std::memcpy(&result_int, &result[0], result.size());
            print("Number: ");
            printi(result_int);
        }
    }
};

EOSIO_DISPATCH(randomsample, (getrandnum)(callback))
