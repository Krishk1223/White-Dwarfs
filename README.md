# Structure of White Dwarfs

[//]: # (Please note that a later verion of this  Raylib is downloaded to be able to run this script effectively otherwise build will fail and you are using C++ 17 or later to compile this code. The build tools makes it so that this should happen by default or it informs you of any underlying issues through errors.)

Uses Equations of state alongside numerical methods to integrate over the central density of white dwarfs using C++ and generate expected masses and expected radii and uses a simple python script to plot the mass-radius relations. Plan to simulate orbits of earth sized objects around these white dwarfs in c++ with raylib soon.

0. Install uv (it will make this much easier): see [uv installation docs](https://docs.astral.sh/uv/getting-started/installation/) for help. Or just have numpy, matplotlib and pandas installed to the latest versions available (in which case skip step 1)
1. Clone directory and set up a virtual environment then ensure installation of relevant packages (with uv run the command below):
```bash
  uv sync
```
2. Select electron fraction (Ye) value in **src** directory's **config.h** file (0.464 for Fe-56 composition or 0.5 for C-12 composition)
3. Run the following commands in the **white_dwarfs** folder (in order):
```bash
  cmake -S -B build
```

  followed by:
```bash
  cmake --build build
```

  finally:
```bash
  ./build/white_dwarfs
```

4. When your csv file is generated run the python script in the **white_dwarfs** folder by using:
```bash
  uv run plots.py
```
  or 
```bash
  python plots.py
```
5. Graphs will be saved in plots/ folder.
