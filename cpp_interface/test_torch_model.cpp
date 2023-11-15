#include "FFISubgridModel.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "H5Cpp.h"

using namespace H5;
using namespace std;

const H5std_string FILE_NAME("test.out.h5"); // input file
const int xgrid = 201;
const int ygrid = 201;
const int zgrid = 101;
//======//
// main //
//======//
int main(int argc, const char* argv[]){
  if (argc != 2) {
    cerr << "usage: example-app <path-to-exported-script-module>\n";
    return -1;
  }
  
  FFISubgridModel<3> model{string(argv[1])};
  
  /*
  input array initialization
  */
  int i, j, k, t;
  float data_out[3][4][1][1];
  for (j = 0; j < 3; j++)
  {
    for (i = 0; i < 4; i++)
    {
      for (k = 0; k < 1; k++)
      {
        for (t = 0; t < 1; t++)
          data_out[j][i][k][t] = 0.0;
      }
    }
  }

  //------------------simulation data-------------------//
  H5File file (FILE_NAME, H5F_ACC_RDONLY); // open simulation file
  
  hsize_t offset[4]; //hyperslab offset in the file
  hsize_t count[4]; //size of the hyperslab in the file;
  offset[0] = 0;
  offset[1] = 0;
  offset[2] = 0;
  offset[3] = 0;
  count[0] = 1;
  count[1] = 3;
  count[2] = 1;
  count[3] = 1;

  hsize_t dimsm[4]; //memory space dimensions
  dimsm[0] = 3;
  dimsm[1] = 4;
  dimsm[2] = 1;
  dimsm[3] = 1;

  DataSet dataset1 = file.openDataSet("/fn_e(1|ccm)");
  DataSpace dataspace1 = dataset1.getSpace();
  dataspace1.selectHyperslab(H5S_SELECT_SET, count, offset);
  DataSpace memspace1(4, dimsm);
  /*
   * Define memory hyperslab
   */
  hsize_t offset_out1[4]; // hyperslab offset in memory
  hsize_t count_out1[4]; // size of the hyperslab in memory
  offset_out1[0] =0; 
  offset_out1[1] =0; 
  offset_out1[2] =0; 
  offset_out1[3] =0; 
  count_out1[0] = 1;
  count_out1[1] = 3;
  count_out1[2] = 1;
  count_out1[3] = 1;
  memspace1.selectHyperslab(H5S_SELECT_SET, count_out1, offset_out1);
  /*
   * Read data from hyperslab in the file into the hyperslab in
   * memory and display the data.
   */
  dataset1.read(data_out, PredType::NATIVE_FLOAT, memspace1, dataspace1);

  DataSet dataset2 = file.openDataSet("/fn_a(1|ccm)");
  DataSpace dataspace2 = dataset2.getSpace();
  dataspace2.selectHyperslab(H5S_SELECT_SET, count, offset);
  DataSpace memspace2(4, dimsm);
  /*
   * Define memory hyperslab
   */
  hsize_t offset_out2[4]; // hyperslab offset in memory
  hsize_t count_out2[4];  // size of the hyperslab in memory
  offset_out2[0] = 1;
  offset_out2[1] = 0;
  offset_out2[2] = 0;
  offset_out2[3] = 0;
  count_out2[0] = 1;
  count_out2[1] = 3;
  count_out2[2] = 1;
  count_out2[3] = 1;
  memspace2.selectHyperslab(H5S_SELECT_SET, count_out2, offset_out2);
  /*
   * Read data from hyperslab in the file into the hyperslab in
   * memory and display the data.
   */
  dataset2.read(data_out, PredType::NATIVE_FLOAT, memspace2, dataspace2);

  DataSet dataset3 = file.openDataSet("/fn_x(1|ccm)");
  DataSpace dataspace3 = dataset3.getSpace();
  dataspace3.selectHyperslab(H5S_SELECT_SET, count, offset);
  DataSpace memspace3(4, dimsm);
  /*
   * Define memory hyperslab
   */
  hsize_t offset_out3[4]; //hyperslab offset in memory
  hsize_t count_out3[4];  //size of the hyperslab in memory
  offset_out3[0] = 2;
  offset_out3[1] = 0; 
  offset_out3[2] = 0; 
  offset_out3[3] = 0;
  count_out3[0] = 1;
  count_out3[1] = 3;
  count_out3[2] = 1;
  count_out3[3] = 1;
  memspace3.selectHyperslab(H5S_SELECT_SET, count_out3, offset_out3);
  /*
   * Read data from hyperslab in the file into the hyperslab in
   * memory and display the data.
   */
  dataset3.read(data_out, PredType::NATIVE_FLOAT, memspace3, dataspace3);

  hsize_t offset1[3]; // hyperslab offset in the file
  hsize_t count1[3];  // size of the hyperslab in the file
  offset1[0] = 0; 
  offset1[1] = 0; 
  offset1[2] = 0; 
  count1[0] = 1;
  count1[1] = 1;
  count1[2] = 1;

  hsize_t dimsm1[3]; // memory space dimensions
  dimsm1[0] = 3;
  dimsm1[1] = 4;
  dimsm1[2] = 1;

  DataSet dataset4 = file.openDataSet("/n_e(1|ccm)");
  DataSpace dataspace4 = dataset4.getSpace();
  dataspace4.selectHyperslab(H5S_SELECT_SET, count1, offset1);
  DataSpace memspace4(3, dimsm1);
  /*
   * Define memory hyperslab
   */
  hsize_t offset_out4[3]; // hyperslab offset in memory
  hsize_t count_out4[3];  // size of the hyperslab in memory
  offset_out4[0] = 0; 
  offset_out4[1] = 3; 
  offset_out4[2] = 0;
  count_out4[0] = 1;
  count_out4[1] = 1;
  count_out4[2] = 1;
  memspace4.selectHyperslab(H5S_SELECT_SET, count_out4, offset_out4);
  /*
   * Read data from hyperslab in the file into the hyperslab in
   * memory and display the data.
   */
  dataset4.read(data_out, PredType::NATIVE_FLOAT, memspace4, dataspace4);
  
  DataSet dataset5 = file.openDataSet("/n_a(1|ccm)");
  DataSpace dataspace5 =dataset5.getSpace();
  dataspace5.selectHyperslab(H5S_SELECT_SET, count1, offset1);
  DataSpace memspace5(3, dimsm1);
  /*
   * Define memory hyperslab
   */
  hsize_t offset_out5[3]; // hyperslab offset in memory
  hsize_t count_out5[3];  // size of the hyperslab in memory
  offset_out5[0] = 1; 
  offset_out5[1] = 3; 
  offset_out5[2] = 0; 
  count_out5[0] = 1;
  count_out5[1] = 1;
  count_out5[2] = 1;
  memspace5.selectHyperslab(H5S_SELECT_SET, count_out5, offset_out5);
  /*
   * Read data from hyperslab in the file into the hyoerslab in
   * memory and display the data.
   */
  dataset5.read(data_out, PredType::NATIVE_FLOAT, memspace5, dataspace5);
  
  DataSet dataset6 = file.openDataSet("/n_x(1|ccm)");
  DataSpace dataspace6 = dataset6.getSpace();
  dataspace6.selectHyperslab(H5S_SELECT_SET, count1, offset1);
  DataSpace memspace6(3, dimsm1);
  /*
   * Define memory hyperslab
   */
  hsize_t offset_out6[3]; // hyperslab offset in memory
  hsize_t count_out6[3]; // size of the hyperslab in memory
  offset_out6[0] = 2;
  offset_out6[1] = 3;
  offset_out6[2] = 0;
  count_out6[0] = 1;
  count_out6[1] = 1;
  count_out6[2] = 1;
  memspace6.selectHyperslab(H5S_SELECT_SET, count_out6, offset_out6);
  /*
   * Read data from hyperslab in the file into the hyperslab in 
   * memory and display the data.
   */
  dataset6.read(data_out, PredType::NATIVE_FLOAT, memspace6, dataspace6);

  //==================================================//
  // Create a sample tensor to pass through the model //
  //==================================================//
  // dimensions are [ngridzones, xyzt, nu/nubar, NF]
  const int ngridzones = 10;
  torch::Tensor F4_in = torch::zeros({ngridzones,4,2,3});
  for (i = 0; i < 2; i++)
  {
    for (j = 0; j < 4; j++)
      F4_in.index_put_({Slice(), j, i, 0}, data_out[i][j][0][0]);
  } 
  for (k = 1; k < 3; k++)
  {
    for (i = 0; i < 2; i++)
    {
      for (j = 0; j < 4; j++)
        F4_in.index_put_({Slice(), j, i, k}, data_out[2][j][0][0]/4.0);
    }
  } 
  torch::Tensor u = torch::zeros({ngridzones,4});
  u.index_put_({Slice(),3}, 1.0);

  // put the input through the model 10 times
  auto output = F4_in;
  for(int i=0; i<10; i++) output = model.predict_F4_Minkowski(output, u);

  // the expected result is an even mixture of all flavors
  //torch::Tensor F4_expected = torch::zeros({ngridzones,4,2,3});
  //F4_expected.index_put_({Slice(), 3, Slice(), Slice()}, 1.0);
  cout << "input" <<F4_in << endl;
  cout << endl;

  // check that the results are correct
  // by asserting that all elements are equal to 1 with an absolute and relative tolerance of 1e-2
  cout << "output" << output << endl;
  //assert(torch::allclose(output, F4_in, 1e-2, 1e-2));

  
  return 0;
}
