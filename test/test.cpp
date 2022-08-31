#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

#include "test_util.h"
#include "hash.h"
#include "serializable_types.h"
#include "signature_builder.h"

BOOST_AUTO_TEST_SUITE(TYPES_TEST)
    BOOST_AUTO_TEST_CASE(headers) {
        Header header_out;
        header_out.count(6);
        header_out.block_size(1024);

        File out("./header_out", std::ios_base::out);
        header_out.serialize(out);
        out.close();

        Header header_in;
        File in("./header_out", std::ios_base::in);
        header_in.deserialize(in);
        in.close();

        BOOST_CHECK_EQUAL(header_in.count(), 6);
        BOOST_CHECK_EQUAL(header_in.block_size(), 1024);
    }

    BOOST_AUTO_TEST_CASE(signature) {
        File old_file = get_file("./old_file", "ABCDEF");
        old_file.seek(std::ios_base::beg);

        Signatures signatures = SignatureBuilder::build(old_file);
        File signature_file("./signature_file", std::ios_base::out);
        signatures.serialize(signature_file);

        BOOST_CHECK_EQUAL(signatures.count(), 2);
    }

    BOOST_AUTO_TEST_CASE(delta_insert_when_new_file_hash_content) {
        Signatures signatures  = get_signatures("");
        Deltas deltas = get_deltas(signatures, "ABCDEFGH");
        std::vector<Delta> deltas_ = deltas.deltas();

        BOOST_CHECK_EQUAL(deltas_.size(), 3);
        BOOST_CHECK_EQUAL(deltas_[0].action(), Action::INSERT);
        BOOST_CHECK_EQUAL(deltas_[1].action(), Action::INSERT);
        BOOST_CHECK_EQUAL(deltas_[2].action(), Action::INSERT);
    }

    BOOST_AUTO_TEST_CASE(delta_delete_when_old_file_is_empty) {
        Signatures signatures  = get_signatures("ABCDEFGH");
        Deltas deltas = get_deltas(signatures, "");
        std::vector<Delta> deltas_ = deltas.deltas();

        BOOST_CHECK_EQUAL(deltas_.size(), 3);
        BOOST_CHECK_EQUAL(deltas_[0].action(), Action::DELETE);
        BOOST_CHECK_EQUAL(deltas_[1].action(), Action::DELETE);
        BOOST_CHECK_EQUAL(deltas_[2].action(), Action::DELETE);
    }

    BOOST_AUTO_TEST_CASE(delta_missing_when_are_equal) {
        Signatures signatures  = get_signatures("ABCDEFGH");
        Deltas deltas = get_deltas(signatures, "ABCDEFGH");
        std::vector<Delta> deltas_ = deltas.deltas();

        BOOST_CHECK_EQUAL(deltas_.size(), 0);
    }

    BOOST_AUTO_TEST_CASE(delta_when_first_block_changed) {
        Signatures signatures  = get_signatures("CBADEFGH");
        Deltas deltas = get_deltas(signatures, "ABCDEFGH");
        std::vector<Delta> deltas_ = deltas.deltas();

        BOOST_CHECK_EQUAL(deltas_.size(), 1);
        BOOST_CHECK_EQUAL(deltas_[0].action(), Action::INSERT);
    }

    BOOST_AUTO_TEST_CASE(delta_when_new_is_less) {
        Signatures signatures = get_signatures("ABCDEFGHIJ");
        Deltas deltas =  get_deltas(signatures, "ABCDEF");
        std::vector<Delta> deltas_ = deltas.deltas();
        BOOST_CHECK_EQUAL(deltas_.size(), 2);
        BOOST_CHECK_EQUAL(deltas_[0].action(), Action::DELETE);
        BOOST_CHECK_EQUAL(deltas_[1].action(), Action::DELETE);
    }

    BOOST_AUTO_TEST_CASE(delta_when_position_is_moved) {
        Signatures signatures = get_signatures("DEFABC");
        Deltas deltas =  get_deltas(signatures, "ABCDEF");
        std::vector<Delta> deltas_ = deltas.deltas();
        BOOST_CHECK_EQUAL(deltas_.size(), 2);
        BOOST_CHECK_EQUAL(deltas_[0].action(), Action::MOVE);
        BOOST_CHECK_EQUAL(deltas_[1].action(), Action::MOVE);
    }

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(HASH_TEST)
    BOOST_AUTO_TEST_CASE(weak_hash_equal) {
        BOOST_CHECK_EQUAL(WeakHash().hash("ABC"), WeakHash().hash("ABC"));
    }

    BOOST_AUTO_TEST_CASE(weak_hash_not_equal) {
        BOOST_CHECK_NE(WeakHash().hash("AAAAAAAAAAAAAAAAS"), WeakHash().hash("ABC"));
    }

BOOST_AUTO_TEST_SUITE_END()