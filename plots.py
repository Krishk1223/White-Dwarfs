import os
import subprocess
import sys
import pandas as pd
import matplotlib.pyplot as plt

def main(generate_data: bool = False):
    if generate_data:
        try:
            # build and run the C++ program to generate the CSV data before plotting
            subprocess.run('cmake -S . -B build && cmake --build build && ./build/white_dwarfs', shell=True, check=True)
        except subprocess.CalledProcessError:
            print("Error: Failed to build or run the white_dwarfs program.")
            sys.exit(1)

    # Load CSV data:
    try:
        data = pd.read_csv('data/white_dwarfs.csv')
    except FileNotFoundError:
        print("Error: 'data/white_dwarfs.csv' not found.")
        sys.exit(1)
    
    if not os.path.exists('plots'):
        os.makedirs('plots')

    # Extract relevant columns:
    electron_fraction = data.iloc[-1, 1]
    data = data.iloc[:-1] #remove last row
    mass = data['Mass_MSUN'].values
    radius = data['Radius_RSUN'].values

    # Example data from literature:
    literature_data = {
        'labels': ['Sirius B', '40 Eri B', 'Stein 2051'],
        'masses': [1.053, 0.48, 0.50],
        'mass_errors': [0.028, 0.02, 0.05],
        'radii': [0.0074, 0.0124, 0.0115],
        'radius_errors': [0.0006, 0.0005, 0.0012]
    }

    literature_df = pd.DataFrame(literature_data)

    # Plot Mass vs Radius:
    plt.figure(figsize=(10, 6))
    plt.plot(mass, radius, color='blue', linewidth=2, label='Calculated M-R Curve', zorder=1)
    
    # Plot example data with error bars
    plt.errorbar(literature_df['masses'], literature_df['radii'], 
                xerr=literature_df['mass_errors'], yerr=literature_df['radius_errors'], 
                fmt='o', markersize=8, color='red', ecolor='red', 
                label='Literature Data', zorder=2)
    
    # Add labels for each point
    for i, label in enumerate(literature_df['labels']):
        plt.annotate(label, (literature_df['masses'].iloc[i], literature_df['radii'].iloc[i]), 
                    textcoords="offset points", xytext=(0,10), ha='center', fontsize=9)
    
    plt.title(f'Mass-Radius Relationship of White Dwarfs (Electron Fraction: {electron_fraction})', fontsize=12)
    plt.xlabel('Mass (Solar Masses)')
    plt.ylabel('Radius (Solar Radii)')
    plt.grid(True, alpha=0.3)
    plt.legend(fontsize=10)
    plt.tight_layout()

    plt.savefig(f'plots/mass_radius_relationship_{electron_fraction}.png')
    print(f"Plot saved to plots/mass_radius_relationship_{electron_fraction}.png")


if __name__ == "__main__":
    aliases = ['--generate', '--generate-data', '-g']
    if len(sys.argv) > 1 and sys.argv[1] in aliases:
        main(generate_data=True)
    else:
        main()
