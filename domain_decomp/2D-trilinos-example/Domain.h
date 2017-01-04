#ifndef DOMAIN_H
#define DOMAIN_H
#include "MyTypeDefs.h"
#include <Epetra_Import.h>
#include <Teuchos_RCP.hpp>
class Domain
{
	public:
	Teuchos::RCP<matrix_type> A;
	Teuchos::RCP<vector_type> f;
	Teuchos::RCP<vector_type> u;
	Teuchos::RCP<solver_type> solver;
	int                       nx;
	int                       ny;
	double                    h_x;
	double                    h_y;
	double *                  boundary_north = nullptr;
	double *                  boundary_south = nullptr;
	double *                  boundary_east  = nullptr;
	double *                  boundary_west  = nullptr;
	bool                      has_north      = false;
	bool                      has_south      = false;
	bool                      has_east       = false;
	bool                      has_west       = false;
	Teuchos::RCP<map_type>    domain_map;

	Domain(Teuchos::RCP<matrix_type> A, Teuchos::RCP<vector_type> f, int nx, int ny, double h_x,
	       double h_y)
	{
		solver = Amesos2::create<matrix_type, vector_type>("KLU2", A);
		solver->symbolicFactorization().numericFactorization();
		this->A   = A;
		this->f   = f;
		this->nx  = nx;
		this->ny  = ny;
		this->h_x = h_x;
		this->h_y = h_y;
		u         = Teuchos::rcp(new vector_type(f->Map(), 1, false));
	}

	void solve()
	{
		Teuchos::RCP<vector_type> f_copy = Teuchos::rcp(new vector_type(f->Map(), 1, false));
		for (int x_i = 0; x_i < nx; x_i++) {
			for (int y_i = 0; y_i < ny; y_i++) {
				(*f_copy)[0][y_i * nx + x_i] = (*f)[0][y_i * nx + x_i];
				if (y_i == ny - 1 && has_north) {
					(*f_copy)[0][y_i * nx + x_i] += -2.0 / (h_y * h_y) * boundary_north[x_i];
				}
				if (x_i == nx - 1 && has_east) {
					(*f_copy)[0][y_i * nx + x_i] += -2.0 / (h_x * h_x) * boundary_east[y_i];
				}
				if (y_i == 0 && has_south) {
					(*f_copy)[0][y_i * nx + x_i] += -2.0 / (h_y * h_y) * boundary_south[x_i];
				}
				if (x_i == 0 && has_west) {
					(*f_copy)[0][y_i * nx + x_i] += -2.0 / (h_x * h_x) * boundary_west[y_i];
				}
			}
		}
		solver->setX(u);
		solver->setB(f_copy);
		solver->solve();
	}

	void solveWithInterface(const vector_type &gamma, vector_type &diff)
	{
		diff.PutScalar(0);
		Epetra_Import importer(*domain_map, gamma.Map());
		vector_type   local_vector(*domain_map, 1);
		local_vector.Import(gamma, importer, Insert);
		int curr_i = 0;
		if (has_north) {
			boundary_north = &local_vector[0][curr_i];
			curr_i += nx;
		}
		if (has_east) {
			boundary_east = &local_vector[0][curr_i];
			curr_i += ny;
		}
		if (has_south) {
			boundary_south = &local_vector[0][curr_i];
			curr_i += nx;
		}
		if (has_west) {
			boundary_west = &local_vector[0][curr_i];
		}

		// solve
		solve();

		local_vector.PutScalar(0.0);
		curr_i = 0;
		if (has_north) {
			for (int i = 0; i < nx; i++) {
				local_vector[0][curr_i] = (*u)[0][nx * (ny - 1) + i];
				curr_i++;
			}
		}
		if (has_east) {
			for (int i = 0; i < ny; i++) {
				local_vector[0][curr_i] = (*u)[0][(i + 1) * nx - 1];
				curr_i++;
			}
		}
		if (has_south) {
			for (int i = 0; i < nx; i++) {
				local_vector[0][curr_i] = (*u)[0][i];
				curr_i++;
			}
		}
		if (has_west) {
			for (int i = 0; i < ny; i++) {
				local_vector[0][curr_i] = (*u)[0][i * nx];
				curr_i++;
			}
		}
		diff.Export(local_vector, importer, Add);
		diff.Update(-2, gamma, 1);
		// diff.Print(std::cout);
		// sleep(1);
	}
};
#endif
