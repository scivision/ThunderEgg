#include "../GMGAvgRstr.h"
#include "../GMGDrctIntp.h"
#include "../InterLevelComm.h"
#include "catch.hpp"
using namespace std;
const int n = 2;
// generate 2 level simple test
void generateTwoLevel(shared_ptr<DomainCollection> &coarse, shared_ptr<DomainCollection> &fine)
{
	// generate simple tree
	OctTree t("3uni.bin");
	coarse.reset(new DomainCollection(t, 2, 2));
	fine.reset(new DomainCollection(t, 3, 2));
}
TEST_CASE("InterLevelComm scatter works", "[GMG]")
{
	PetscInitialize(nullptr, nullptr, nullptr, nullptr);
	{
		shared_ptr<DomainCollection> coarse;
		shared_ptr<DomainCollection> fine;
		generateTwoLevel(coarse, fine);
		shared_ptr<InterLevelComm> comm(new InterLevelComm(coarse, fine));
		PW<Vec>                    coarse_expected       = coarse->getNewDomainVec();
		PW<Vec>                    coarse_local_expected = comm->getNewCoarseDistVec();
		double *                   ce_vec;
		VecGetArray(coarse_expected, &ce_vec);
		for (auto p : coarse->domains) {
			Domain &d = *p.second;
			for (int i = 0; i < n * n * n; i++) {
				ce_vec[d.id_local * n * n * n + i] = d.id;
			}
		}
		VecRestoreArray(coarse_expected, &ce_vec);
		double coarse_expected_norm;
		VecNorm(coarse_expected, NORM_2, &coarse_expected_norm);

		VecGetArray(coarse_local_expected, &ce_vec);
		for (auto data : comm->getFineDomains()) {
			Domain &d = *data.d;
			for (int i = 0; i < n * n * n; i++) {
				ce_vec[data.local_index * n * n * n + i] = d.parent_id;
			}
		}
		VecRestoreArray(coarse_local_expected, &ce_vec);
		double coarse_local_expected_norm;
		VecNorm(coarse_local_expected, NORM_2, &coarse_local_expected_norm);

		// check that forward scatter works as expected
		PW<VecScatter> scatter             = comm->getScatter();
		PW<Vec>        coarse_local_result = comm->getNewCoarseDistVec();
		VecScatterBegin(scatter, coarse_expected, coarse_local_result, ADD_VALUES, SCATTER_FORWARD);
		VecScatterEnd(scatter, coarse_expected, coarse_local_result, ADD_VALUES, SCATTER_FORWARD);

		PetscBool vec_scatter_forward_works;
		VecEqual(coarse_local_result, coarse_local_expected, &vec_scatter_forward_works);
		REQUIRE(vec_scatter_forward_works == PETSC_TRUE);

		double coarse_local_result_norm;
		VecNorm(coarse_local_result, NORM_2, &coarse_local_result_norm);
		REQUIRE(coarse_local_result_norm == coarse_local_expected_norm);

		// check that reverse scatter works as expected
		PW<Vec> coarse_result = coarse->getNewDomainVec();
		VecScatterBegin(scatter, coarse_local_expected, coarse_result, ADD_VALUES, SCATTER_REVERSE);
		VecScatterEnd(scatter, coarse_local_expected, coarse_result, ADD_VALUES, SCATTER_REVERSE);

		PetscBool vec_scatter_reverse_works;
		VecEqual(coarse_result, coarse_expected, &vec_scatter_reverse_works);
		REQUIRE(vec_scatter_forward_works == PETSC_TRUE);

		double coarse_result_norm;
		VecNorm(coarse_result, NORM_2, &coarse_result_norm);
		REQUIRE(coarse_result_norm == coarse_expected_norm);
	}
}
TEST_CASE("GMGAvgRstr works", "[GMG]")
{
	PetscInitialize(nullptr, nullptr, nullptr, nullptr);
	{
		shared_ptr<DomainCollection> coarse;
		shared_ptr<DomainCollection> fine;
		generateTwoLevel(coarse, fine);
		shared_ptr<InterLevelComm> comm(new InterLevelComm(coarse, fine));
		shared_ptr<GMGRestrictor>  op(new GMGAvgRstr(coarse, fine, comm));
		PW<Vec>                    coarse_expected = coarse->getNewDomainVec();
		PW<Vec>                    fine_start      = fine->getNewDomainVec();
		double *                   ce_vec;
		VecGetArray(coarse_expected, &ce_vec);
		for (auto p : coarse->domains) {
			Domain &d = *p.second;
			for (int i = 0; i < n * n * n; i++) {
				ce_vec[d.id_local * n * n * n + i] = d.id;
			}
		}
		VecRestoreArray(coarse_expected, &ce_vec);
		double coarse_expected_norm;
		VecNorm(coarse_expected, NORM_2, &coarse_expected_norm);

		VecGetArray(fine_start, &ce_vec);
		for (auto data : fine->domains) {
			Domain &d = *data.second;
			for (int i = 0; i < n * n * n; i++) {
				ce_vec[d.id_local * n * n * n + i] = d.parent_id;
			}
		}
		VecRestoreArray(fine_start, &ce_vec);

		// check that restrictor works
		PW<Vec> coarse_result = coarse->getNewDomainVec();
        op->restrict(coarse_result,fine_start);

		PetscBool restrictor_works;
		VecEqual(coarse_result, coarse_expected, &restrictor_works);
		REQUIRE(restrictor_works == PETSC_TRUE);

		double coarse_result_norm;
		VecNorm(coarse_result, NORM_2, &coarse_result_norm);
		REQUIRE(coarse_result_norm == coarse_expected_norm);
		REQUIRE(coarse_result_norm != 0);
	}
}
TEST_CASE("GMGDrctIntp works", "[GMG]") {
	PetscInitialize(nullptr, nullptr, nullptr, nullptr);
	{
		shared_ptr<DomainCollection> coarse;
		shared_ptr<DomainCollection> fine;
		generateTwoLevel(coarse, fine);
		shared_ptr<InterLevelComm> comm(new InterLevelComm(coarse, fine));
		shared_ptr<GMGInterpolator>  op(new GMGDrctIntp(coarse, fine, comm));
		PW<Vec>                    coarse_start = coarse->getNewDomainVec();
		PW<Vec>                    fine_expected      = fine->getNewDomainVec();
		double *                   ce_vec;
		VecGetArray(coarse_start, &ce_vec);
		for (auto p : coarse->domains) {
			Domain &d = *p.second;
			for (int i = 0; i < n * n * n; i++) {
				ce_vec[d.id_local * n * n * n + i] = d.id;
			}
		}
		VecRestoreArray(coarse_start, &ce_vec);

		VecGetArray(fine_expected, &ce_vec);
		for (auto data : fine->domains) {
			Domain &d = *data.second;
			for (int i = 0; i < n * n * n; i++) {
				ce_vec[d.id_local * n * n * n + i] = d.parent_id;
			}
		}
		VecRestoreArray(fine_expected, &ce_vec);
		double fine_expected_norm;
		VecNorm(fine_expected, NORM_2, &fine_expected_norm);

		// check that restrictor works
		PW<Vec> fine_result = fine->getNewDomainVec();
        op->interpolate(coarse_start,fine_result);

		PetscBool restrictor_works;
		VecEqual(fine_result, fine_expected, &restrictor_works);
		REQUIRE(restrictor_works == PETSC_TRUE);

		double fine_result_norm;
		VecNorm(fine_result, NORM_2, &fine_result_norm);
		REQUIRE(fine_result_norm == fine_expected_norm);
		REQUIRE(fine_result_norm != 0);
	}
	PetscFinalize();
}
