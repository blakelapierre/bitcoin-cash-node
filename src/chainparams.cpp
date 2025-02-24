// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2017-2021 The Bitcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>

#include <chainparamsconstants.h>
#include <chainparamsseeds.h>
#include <consensus/consensus.h>
#include <consensus/merkle.h>
#include <netbase.h>
#include <tinyformat.h>
#include <util/strencodings.h>
#include <util/system.h>

#include <cassert>
#include <cstring>
#include <memory>
#include <stdexcept>

static CBlock CreateGenesisBlock(const char *pszTimestamp,
                                 const CScript &genesisOutputScript,
                                 uint32_t nTime, uint32_t nNonce,
                                 uint32_t nBits, int32_t nVersion,
                                 const Amount genesisReward) {
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig =
        CScript() << ScriptInt::fromIntUnchecked(486604799)
                  << CScriptNum::fromIntUnchecked(4)
                  << std::vector<uint8_t>((const uint8_t *)pszTimestamp,
                                          (const uint8_t *)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime = nTime;
    genesis.nBits = nBits;
    genesis.nNonce = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation transaction
 * cannot be spent since it did not originally exist in the database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000,
 * hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893,
 * vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase
 * 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits,
                          int32_t nVersion, const Amount genesisReward) {
    const char *pszTimestamp =
        "hi twitter";
    const CScript genesisOutputScript =
        CScript() << ParseHex("04855b21edaac2ed5a5690674dc2f925321b2d33bf8acc22"
                              "fa86b36f37a268e0ee5a5f37fab6d0b04bb56762d54aa683"
                              "b86ae23857b4ad8a17287ed302afc37871")
                  << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce,
                              nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = CBaseChainParams::MAIN;
        consensus.nSubsidyHalvingInterval = 210000;
        // 00000000000000ce80a7e057163a4db1d5ad7b20fb6f598c9597b9665c8fb0d4 -
        // April 1, 2012
        consensus.BIP16Height = 1;
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = BlockHash::fromHex(
            "00000000671e78e128aa4b3943689e2675d80410c280a81d9e102036716f1a0e");
        // 000000000000000004c2b624ed5d7756c508d90fd0da2c7c679febfa6c4735f0
        consensus.BIP65Height = 1;
        // 00000000000000000379eaa19dce8c9b722d46ae6a57c2f1a988119488b50931
        consensus.BIP66Height = 1;
        // 000000000000000004a1b34462cb8aeebd5799177f7a29cf28f2d1961716b5b5
        consensus.CSVHeight = 1;
        consensus.powLimit = uint256S(
            "00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        // two weeks
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60;
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;

        // The half life for the ASERT DAA. For every (nASERTHalfLife) seconds behind schedule the blockchain gets,
        // difficulty is cut in half. Doubled if blocks are ahead of schedule.
        // Two days
        consensus.nASERTHalfLife = 2 * 24 * 60 * 60;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork =
            ChainParamsConstants::MAINNET_MINIMUM_CHAIN_WORK;

        // By default assume that the signatures in ancestors of this block are
        // valid.
        consensus.defaultAssumeValid =
            ChainParamsConstants::MAINNET_DEFAULT_ASSUME_VALID;

        // August 1, 2017 hard fork
        consensus.uahfHeight = 1;

        // November 13, 2017 hard fork
        consensus.daaHeight = 1;

        // November 15, 2018 hard fork
        consensus.magneticAnomalyHeight = 1;

        // November 15, 2019 protocol upgrade
        consensus.gravitonHeight = 1;

        // May 15, 2020 12:00:00 UTC protocol upgrade
        consensus.phononHeight = 1;

        // Nov 15, 2020 12:00:00 UTC protocol upgrade
        consensus.axionActivationTime = 1;

        // May 15, 2021 12:00:00 UTC protocol upgrade was 1621080000, but since this upgrade was for relay rules only,
        // we do not track this time (since it does not apply at all to the blockchain itself).

        // May 15, 2022 12:00:00 UTC protocol upgrade
        consensus.upgrade8ActivationTime = 1;

        // May 15, 2023 12:00:00 UTC tentative protocol upgrade
        consensus.upgrade9ActivationTime = 1;

        // Default limit for block size (in bytes)
        consensus.nDefaultExcessiveBlockSize = DEFAULT_EXCESSIVE_BLOCK_SIZE;

        // Chain-specific default for mining block size (in bytes) (configurable with -blockmaxsize)
        consensus.nDefaultGeneratedBlockSize = 8 * ONE_MEGABYTE;

        assert(consensus.nDefaultGeneratedBlockSize <= consensus.nDefaultExcessiveBlockSize);

        // Anchor params: Note that the block after this height *must* also be checkpointed below.
        consensus.asertAnchorParams = Consensus::Params::ASERTAnchor{
            661647,       // anchor block height
            0x1804dafe,   // anchor block nBits
            1605447844,   // anchor block previous block timestamp
        };

        /**
         * The message start string is designed to be unlikely to occur in
         * normal data. The characters are rarely used upper ASCII, not valid as
         * UTF-8, and produce a large 32-bit integer with any alignment.
         */
        diskMagic[0] = 0xfa;
        diskMagic[1] = 0xbf;
        diskMagic[2] = 0xb5;
        diskMagic[3] = 0xda;
        netMagic[0] = 0xe4;
        netMagic[1] = 0xe2;
        netMagic[2] = 0xf4;
        netMagic[3] = 0xe9;
        nDefaultPort = 8333;
        nPruneAfterHeight = 100000;
        m_assumed_blockchain_size = 240;
        m_assumed_chain_state_size = 5;

        genesis = CreateGenesisBlock(1638317164, 1607403406, 0x1d00ffff, 1,
                                     50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock ==
               uint256S("00000000671e78e128aa4b3943689e2675d80410c280a81d9e1020"
                        "36716f1a0e"));
        assert(genesis.hashMerkleRoot ==
               uint256S("47e610f3c0282e7318658c16dfbb6e34b32a8d16b41b36ff693a46"
                        "0d50967077"));

        vSeeds.emplace_back("seed.flowee.cash");
        // Note that of those which support the service bits prefix, most only
        // support a subset of possible options. This is fine at runtime as
        // we'll fall back to using them as a oneshot if they don't support the
        // service bits we want, but we should get them updated to support all
        // service bits wanted by any release ASAP to avoid it where possible.
        // bitcoinforks seeders
        vSeeds.emplace_back("seed-bch.bitcoinforks.org");
        // BU backed seeder
        vSeeds.emplace_back("btccash-seeder.bitcoinunlimited.info");
        // BCHD
        vSeeds.emplace_back("seed.bchd.cash");
        // Loping.net
        vSeeds.emplace_back("seed.bch.loping.net");
        // Electroncash.de
        vSeeds.emplace_back("dnsseed.electroncash.de");
        // C3 Soft (NilacTheGrim)
        vSeeds.emplace_back("bchseed.c3-soft.com");
        // Jason Dreyzehner
        vSeeds.emplace_back("bch.bitjson.com");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<uint8_t>(1, 0);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<uint8_t>(1, 5);
        base58Prefixes[SECRET_KEY] = std::vector<uint8_t>(1, 128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};
        cashaddrPrefix = "tbj";

        vFixedSeeds.assign(std::begin(pnSeed6_main), std::end(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        m_is_test_chain = false;

        checkpointData = { };

        // Data as of block
        // 000000000000000000d7e938f43eb520468fc75dc626c54ec770f9cd1bd6bc1d
        // (height 699219).
        chainTxData = ChainTxData{
            // UNIX timestamp of last known number of transactions.
            0,
            // Total number of transactions between genesis and that timestamp
            // (the tx=... number in the ChainStateFlushed debug.log lines)
            0,
            // Estimated number of transactions per second after that timestamp.
            0,
        };
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = CBaseChainParams::TESTNET;
        consensus.nSubsidyHalvingInterval = 210000;
        // 00000000040b4e986385315e14bee30ad876d8b47f748025b26683116d21aa65
        consensus.BIP16Height = 1;
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = BlockHash::fromHex(
            "00000000671e78e128aa4b3943689e2675d80410c280a81d9e102036716f1a0e");
        // 00000000007f6655f22f98e72ed80d8b06dc761d5da09df0fa1dc4be4f861eb6
        consensus.BIP65Height = 1;
        // 000000002104c8c45e99a8853285a3b592602a3ccde2b832481da85e9e4ba182
        consensus.BIP66Height = 1;
        // 00000000025e930139bac5c6c31a403776da130831ab85be56578f3fa75369bb
        consensus.CSVHeight = 1;
        consensus.powLimit = uint256S(
            "00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        // two weeks
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60;
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;

        // The half life for the ASERT DAA. For every (nASERTHalfLife) seconds behind schedule the blockchain gets,
        // difficulty is cut in half. Doubled if blocks are ahead of schedule.
        // One hour
        consensus.nASERTHalfLife = 60 * 60;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork =
            ChainParamsConstants::TESTNET_MINIMUM_CHAIN_WORK;

        // By default assume that the signatures in ancestors of this block are
        // valid.
        consensus.defaultAssumeValid =
            ChainParamsConstants::TESTNET_DEFAULT_ASSUME_VALID;

        // August 1, 2017 hard fork
        consensus.uahfHeight = 1;

        // November 13, 2017 hard fork
        consensus.daaHeight = 1;

        // November 15, 2018 hard fork
        consensus.magneticAnomalyHeight = 1;

        // November 15, 2019 protocol upgrade
        consensus.gravitonHeight = 1;

        // May 15, 2020 12:00:00 UTC protocol upgrade
        consensus.phononHeight = 1;

        // Nov 15, 2020 12:00:00 UTC protocol upgrade
        consensus.axionActivationTime = 1;

        // May 15, 2022 12:00:00 UTC protocol upgrade
        consensus.upgrade8ActivationTime = 1;

        // May 15, 2023 12:00:00 UTC tentative protocol upgrade
        consensus.upgrade9ActivationTime = 1;

        // Default limit for block size (in bytes)
        consensus.nDefaultExcessiveBlockSize = DEFAULT_EXCESSIVE_BLOCK_SIZE;

        // Chain-specific default for mining block size (in bytes) (configurable with -blockmaxsize)
        consensus.nDefaultGeneratedBlockSize = 8 * ONE_MEGABYTE;

        assert(consensus.nDefaultGeneratedBlockSize <= consensus.nDefaultExcessiveBlockSize);

        // Anchor params: Note that the block after this height *must* also be checkpointed below.
        consensus.asertAnchorParams = Consensus::Params::ASERTAnchor{
            1421481,      // anchor block height
            0x1d00ffff,   // anchor block nBits
            1605445400,   // anchor block previous block timestamp
        };

        diskMagic[0] = 0xfa;
        diskMagic[1] = 0xbf;
        diskMagic[2] = 0xb5;
        diskMagic[3] = 0xda;
        netMagic[0] = 0xe4;
        netMagic[1] = 0xe2;
        netMagic[2] = 0xf4;
        netMagic[3] = 0xe9;
        nDefaultPort = 18333;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 60;
        m_assumed_chain_state_size = 2;

        genesis = CreateGenesisBlock(1638317164, 1607403406, 0x1d00ffff, 1,
                                     50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock ==
               uint256S("00000000671e78e128aa4b3943689e2675d80410c280a81d9e1020"
                        "36716f1a0e"));
        assert(genesis.hashMerkleRoot ==
               uint256S("47e610f3c0282e7318658c16dfbb6e34b32a8d16b41b36ff693a46"
                        "0d50967077"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        // bitcoinforks seeders
        vSeeds.emplace_back("testnet-seed-bch.bitcoinforks.org");
        // BCHD
        vSeeds.emplace_back("testnet-seed.bchd.cash");
        // Loping.net
        vSeeds.emplace_back("seed.tbch.loping.net");
        // Bitcoin Unlimited
        vSeeds.emplace_back("testnet-seed.bitcoinunlimited.info");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<uint8_t>(1, 111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<uint8_t>(1, 196);
        base58Prefixes[SECRET_KEY] = std::vector<uint8_t>(1, 239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
        cashaddrPrefix = "bchtest";
        vFixedSeeds.assign(std::begin(pnSeed6_testnet3), std::end(pnSeed6_testnet3));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        m_is_test_chain = true;

        checkpointData = {};

        // Data as of block
        // 0000000000002ad25634e653f5834f0c710fab41891dd696bf504262745e5cd5
        // (height 1459224)
        chainTxData = ChainTxData{0, 0, 0};
    }
};

/**
 * Testnet (v4)
 */
class CTestNet4Params : public CChainParams {
public:
    CTestNet4Params() {
        strNetworkID = CBaseChainParams::TESTNET4;
        consensus.nSubsidyHalvingInterval = 210000;
        consensus.BIP16Height = 1;
        // Note: Because BIP34Height is less than 17, clients will face an unusual corner case with BIP34 encoding.
        // The "correct" encoding for BIP34 blocks at height <= 16 uses OP_1 (0x81) through OP_16 (0x90) as a single
        // byte (i.e. "[shortest possible] encoded CScript format"), not a single byte with length followed by the
        // little-endian encoded version of the height as mentioned in BIP34. The BIP34 spec document itself ought to
        // be updated to reflect this.
        // https://github.com/bitcoin/bitcoin/pull/14633
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = BlockHash::fromHex("00000000671e78e128aa4b3943689e2675d80410c280a81d9e102036716f1a0e");
        consensus.BIP65Height = 1;
        consensus.BIP66Height = 1;
        consensus.CSVHeight = 1;
        consensus.powLimit = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        // two weeks
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60;
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;

        // The half life for the ASERT DAA. For every (nASERTHalfLife) seconds behind schedule the blockchain gets,
        // difficulty is cut in half. Doubled if blocks are ahead of schedule.
        // One hour
        consensus.nASERTHalfLife = 60 * 60;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = ChainParamsConstants::TESTNET4_MINIMUM_CHAIN_WORK;

        // By default assume that the signatures in ancestors of this block are
        // valid.
        consensus.defaultAssumeValid = ChainParamsConstants::TESTNET4_DEFAULT_ASSUME_VALID;

        // August 1, 2017 hard fork
        consensus.uahfHeight = 1;

        // November 13, 2017 hard fork
        consensus.daaHeight = 1;

        // November 15, 2018 hard fork
        consensus.magneticAnomalyHeight = 1;

        // November 15, 2019 protocol upgrade
        consensus.gravitonHeight = 1;

        // May 15, 2020 12:00:00 UTC protocol upgrade
        // Note: We must set this to 0 here because "historical" sigop code has
        //       been removed from the BCHN codebase. All sigop checks really
        //       use the new post-May2020 sigcheck code unconditionally in this
        //       codebase, regardless of what this height is set to. So it's
        //       "as-if" the activation height really is 0 for all intents and
        //       purposes. If other node implementations wish to use this code
        //       as a reference, they need to be made aware of this quirk of
        //       BCHN, so we explicitly set the activation height to zero here.
        //       For example, BU or other nodes do keep both sigop and sigcheck
        //       implementations in their execution paths so they will need to
        //       use 0 here to be able to synch to this chain.
        //       See: https://gitlab.com/bitcoin-cash-node/bitcoin-cash-node/-/issues/167
        consensus.phononHeight = 0;

        // Nov 15, 2020 12:00:00 UTC protocol upgrade
        consensus.axionActivationTime = 1;

        // May 15, 2022 12:00:00 UTC protocol upgrade
        consensus.upgrade8ActivationTime = 1;

        // May 15, 2023 12:00:00 UTC tentative protocol upgrade
        consensus.upgrade9ActivationTime = 1;

        // Default limit for block size (in bytes) (testnet4 is smaller at 2MB)
        consensus.nDefaultExcessiveBlockSize = 2 * ONE_MEGABYTE;

        // Chain-specific default for mining block size (in bytes) (configurable with -blockmaxsize)
        consensus.nDefaultGeneratedBlockSize = 2 * ONE_MEGABYTE;

        assert(consensus.nDefaultGeneratedBlockSize <= consensus.nDefaultExcessiveBlockSize);

        // Anchor params: Note that the block after this height *must* also be checkpointed below.
        consensus.asertAnchorParams = Consensus::Params::ASERTAnchor{
            16844,        // anchor block height
            0x1d00ffff,   // anchor block nBits
            1605451779,   // anchor block previous block timestamp
        };

        diskMagic[0] = 0xfa;
        diskMagic[1] = 0xbf;
        diskMagic[2] = 0xb5;
        diskMagic[3] = 0xda;
        netMagic[0] = 0xe4;
        netMagic[1] = 0xe2;
        netMagic[2] = 0xf4;
        netMagic[3] = 0xe9;
        nDefaultPort = 28333;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 1;
        m_assumed_chain_state_size = 1;

        genesis = CreateGenesisBlock(1638317164, 1607403406, 0x1d00ffff, 1,
                                     50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock ==
               uint256S("00000000671e78e128aa4b3943689e2675d80410c280a81d9e1020"
                        "36716f1a0e"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("testnet4-seed-bch.bitcoinforks.org");
        vSeeds.emplace_back("testnet4-seed-bch.toom.im");
        // Loping.net
        vSeeds.emplace_back("seed.tbch4.loping.net");
        // Flowee
        vSeeds.emplace_back("testnet4-seed.flowee.cash");
        // Jason Dreyzehner
        vSeeds.emplace_back("testnet4.bitjson.com");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<uint8_t>(1, 111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<uint8_t>(1, 196);
        base58Prefixes[SECRET_KEY] = std::vector<uint8_t>(1, 239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
        cashaddrPrefix = "bchtest";
        vFixedSeeds.assign(std::begin(pnSeed6_testnet4), std::end(pnSeed6_testnet4));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        m_is_test_chain = true;

        checkpointData = {};

        // Data as of block
        // 00000000009758d51aaf3bdc018b8b5c6e1725f742c850d44a0585ec168c409d
        // (height 54516)
        chainTxData = {0, 0, 0};
    }
};

/**
 * Scalenet
 */
class CScaleNetParams : public CChainParams {
public:
    CScaleNetParams() {
        strNetworkID = CBaseChainParams::SCALENET;
        consensus.nSubsidyHalvingInterval = 210000;
        consensus.BIP16Height = 1;
        consensus.BIP34Height = 1;
        // Note: Because BIP34Height is less than 17, clients will face an unusual corner case with BIP34 encoding.
        // The "correct" encoding for BIP34 blocks at height <= 16 uses OP_1 (0x81) through OP_16 (0x90) as a single
        // byte (i.e. "[shortest possible] encoded CScript format"), not a single byte with length followed by the
        // little-endian encoded version of the height as mentioned in BIP34. The BIP34 spec document itself ought to
        // be updated to reflect this.
        // https://github.com/bitcoin/bitcoin/pull/14633
        consensus.BIP34Hash = BlockHash::fromHex("00000000c8c35eaac40e0089a83bf5c5d9ecf831601f98c21ed4a7cb511a07d8");
        consensus.BIP65Height = 1;
        consensus.BIP66Height = 1;
        consensus.CSVHeight = 1;
        consensus.powLimit = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        // two weeks
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60;
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;

        // The half life for the ASERT DAA. For every (nASERTHalfLife) seconds behind schedule the blockchain gets,
        // difficulty is cut in half. Doubled if blocks are ahead of schedule.
        // Two days
        consensus.nASERTHalfLife = 2 * 24 * 60 * 60;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = ChainParamsConstants::SCALENET_MINIMUM_CHAIN_WORK;

        // By default assume that the signatures in ancestors of this block are
        // valid.
        consensus.defaultAssumeValid = ChainParamsConstants::SCALENET_DEFAULT_ASSUME_VALID;

        // August 1, 2017 hard fork
        consensus.uahfHeight = 1;

        // November 13, 2017 hard fork
        consensus.daaHeight = 1;

        // November 15, 2018 hard fork
        consensus.magneticAnomalyHeight = 1;

        // November 15, 2019 protocol upgrade
        consensus.gravitonHeight = 1;

        // May 15, 2020 12:00:00 UTC protocol upgrade
        // Note: We must set this to 0 here because "historical" sigop code has
        //       been removed from the BCHN codebase. All sigop checks really
        //       use the new post-May2020 sigcheck code unconditionally in this
        //       codebase, regardless of what this height is set to. So it's
        //       "as-if" the activation height really is 0 for all intents and
        //       purposes. If other node implementations wish to use this code
        //       as a reference, they need to be made aware of this quirk of
        //       BCHN, so we explicitly set the activation height to zero here.
        //       For example, BU or other nodes do keep both sigop and sigcheck
        //       implementations in their execution paths so they will need to
        //       use 0 here to be able to synch to this chain.
        //       See: https://gitlab.com/bitcoin-cash-node/bitcoin-cash-node/-/issues/167
        consensus.phononHeight = 0;

        // Nov 15, 2020 12:00:00 UTC protocol upgrade
        consensus.axionActivationTime = 1;

        // May 15, 2022 12:00:00 UTC protocol upgrade
        consensus.upgrade8ActivationTime = 1;

        // May 15, 2023 12:00:00 UTC tentative protocol upgrade
        consensus.upgrade9ActivationTime = 1;

        // Default limit for block size (in bytes)
        consensus.nDefaultExcessiveBlockSize = 256 * ONE_MEGABYTE;

        // Chain-specific default for mining block size (in bytes) (configurable with -blockmaxsize)
        consensus.nDefaultGeneratedBlockSize = 8 * ONE_MEGABYTE;

        assert(consensus.nDefaultGeneratedBlockSize <= consensus.nDefaultExcessiveBlockSize);

        // ScaleNet has no hard-coded anchor block because will be expected to
        // reorg back down to height 10,000 periodically.
        consensus.asertAnchorParams.reset();

        diskMagic[0] = 0xfa;
        diskMagic[1] = 0xbf;
        diskMagic[2] = 0xb5;
        diskMagic[3] = 0xda;
        netMagic[0] = 0xe4;
        netMagic[1] = 0xe2;
        netMagic[2] = 0xf4;
        netMagic[3] = 0xe9;
        nDefaultPort = 38333;
        nPruneAfterHeight = 10000;
        m_assumed_blockchain_size = 200;
        m_assumed_chain_state_size = 20;

        genesis = CreateGenesisBlock(1638317164, 1607403406, 0x1d00ffff, 1,
                                     50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock ==
               uint256S("00000000671e78e128aa4b3943689e2675d80410c280a81d9e1020"
                        "36716f1a0e"));
        assert(genesis.hashMerkleRoot ==
               uint256S("47e610f3c0282e7318658c16dfbb6e34b32a8d16b41b36ff693a46"
                        "0d50967077"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        // bitcoinforks seeders
        vSeeds.emplace_back("scalenet-seed-bch.bitcoinforks.org");
        vSeeds.emplace_back("scalenet-seed-bch.toom.im");
        // Loping.net
        vSeeds.emplace_back("seed.sbch.loping.net");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<uint8_t>(1, 111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<uint8_t>(1, 196);
        base58Prefixes[SECRET_KEY] = std::vector<uint8_t>(1, 239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
        cashaddrPrefix = "bchtest";
        vFixedSeeds.assign(std::begin(pnSeed6_scalenet), std::end(pnSeed6_scalenet));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        m_is_test_chain = true;

        checkpointData = {};

        chainTxData = {0, 0, 0};
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = CBaseChainParams::REGTEST;
        consensus.nSubsidyHalvingInterval = 150;
        // always enforce P2SH BIP16 on regtest
        consensus.BIP16Height = 0;
        // BIP34 has not activated on regtest (far in the future so block v1 are
        // not rejected in tests)
        consensus.BIP34Height = 100000000;
        consensus.BIP34Hash = BlockHash();
        // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP65Height = 1351;
        // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251;
        // CSV activated on regtest (Used in rpc activation tests)
        consensus.CSVHeight = 576;
        consensus.powLimit = uint256S(
            "7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        // two weeks
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60;
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;

        // The half life for the ASERT DAA. For every (nASERTHalfLife) seconds behind schedule the blockchain gets,
        // difficulty is cut in half. Doubled if blocks are ahead of schedule.
        // Two days. Note regtest has no DAA checks, so this unused parameter is here merely for completeness.
        consensus.nASERTHalfLife = 2 * 24 * 60 * 60;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are
        // valid.
        consensus.defaultAssumeValid = BlockHash();

        // UAHF is always enabled on regtest.
        consensus.uahfHeight = 0;

        // November 13, 2017 hard fork is always on on regtest.
        consensus.daaHeight = 0;

        // November 15, 2018 hard fork is always on on regtest.
        consensus.magneticAnomalyHeight = 0;

        // November 15, 2019 protocol upgrade
        consensus.gravitonHeight = 0;

        // May 15, 2020 12:00:00 UTC protocol upgrade
        consensus.phononHeight = 0;

        // Nov 15, 2020 12:00:00 UTC protocol upgrade
        consensus.axionActivationTime = 1605441600;

        // May 15, 2022 12:00:00 UTC protocol upgrade
        consensus.upgrade8ActivationTime = 1652616000;

        // May 15, 2023 12:00:00 UTC tentative protocol upgrade
        consensus.upgrade9ActivationTime = 1684152000;

        // Default limit for block size (in bytes)
        consensus.nDefaultExcessiveBlockSize = DEFAULT_EXCESSIVE_BLOCK_SIZE;

        // Chain-specific default for mining block size (in bytes) (configurable with -blockmaxsize)
        consensus.nDefaultGeneratedBlockSize = 8 * ONE_MEGABYTE;

        assert(consensus.nDefaultGeneratedBlockSize <= consensus.nDefaultExcessiveBlockSize);

        diskMagic[0] = 0xfa;
        diskMagic[1] = 0xbf;
        diskMagic[2] = 0xb5;
        diskMagic[3] = 0xda;
        netMagic[0] = 0xe4;
        netMagic[1] = 0xe2;
        netMagic[2] = 0xf4;
        netMagic[3] = 0xe9;
        nDefaultPort = 18444;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 0;
        m_assumed_chain_state_size = 0;

        genesis = CreateGenesisBlock(1638317164, 1607403406, 0x1d00ffff, 1,
                                     50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock ==
               uint256S("00000000671e78e128aa4b3943689e2675d80410c280a81d9e1020"
                        "36716f1a0e"));
        assert(genesis.hashMerkleRoot ==
               uint256S("47e610f3c0282e7318658c16dfbb6e34b32a8d16b41b36ff693a46"
                        "0d50967077"));

        //! Regtest mode doesn't have any fixed seeds.
        vFixedSeeds.clear();
        //! Regtest mode doesn't have any DNS seeds.
        vSeeds.clear();

        fDefaultConsistencyChecks = true;
        fRequireStandard = true;
        m_is_test_chain = true;

        checkpointData = {};

        chainTxData = ChainTxData{0, 0, 0};

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<uint8_t>(1, 111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<uint8_t>(1, 196);
        base58Prefixes[SECRET_KEY] = std::vector<uint8_t>(1, 239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
        cashaddrPrefix = "bchreg";
    }
};


static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string &chain) {
    if (chain == CBaseChainParams::MAIN) {
        return std::make_unique<CMainParams>();
    }

    if (chain == CBaseChainParams::TESTNET) {
        return std::make_unique<CTestNetParams>();
    }

    if (chain == CBaseChainParams::TESTNET4) {
        return std::make_unique<CTestNet4Params>();
    }

    if (chain == CBaseChainParams::REGTEST) {
        return std::make_unique<CRegTestParams>();
    }

    if (chain == CBaseChainParams::SCALENET) {
        return std::make_unique<CScaleNetParams>();
    }

    throw std::runtime_error(
        strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string &network) {
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

SeedSpec6::SeedSpec6(const char *pszHostPort)
{
    const CService service = LookupNumeric(pszHostPort, 0);
    if (!service.IsValid() || service.GetPort() == 0)
        throw std::invalid_argument(strprintf("Unable to parse numeric-IP:port pair: %s", pszHostPort));
    if (!service.IsRoutable())
        throw std::invalid_argument(strprintf("Not routable: %s", pszHostPort));
    *this = SeedSpec6(service);
}
