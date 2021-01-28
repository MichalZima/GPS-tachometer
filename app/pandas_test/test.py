# To add a new cell, type '# %%'
# To add a new markdown cell, type '# %% [markdown]'
# %%
import pandas as pd


# %%
from matplotlib import pyplot as plt


# %%
x = [1, 2, 3]
y = [1, 4, 9]
z = [10, 5, 0]
plt.plot(x, y)
plt.plot(x, z)
plt.title("test plot")
plt.xlabel("x")
plt.ylabel("y and z")
plt.legend(["this is y", "this is z"])


# %%
sample_data = pd.read_csv('sample_data.csv')


# %%
sample_data


# %%
type(sample_data)


# %%
sample_data.column_c.iloc[0]


# %%
plt.plot(sample_data.column_a, sample_data.column_b, 'o')
plt.plot(sample_data.column_a, sample_data.column_c)
plt.show()


# %%
data = pd.read_csv('countries.csv')


# %%
data


# %%
# Compare the population growth in the US and China


# %%
data[data.country == 'United States']


# %%
us = data[data.country == 'United States']


# %%
china = data[data.country == 'China']


# %%
china


# %%
plt.plot(us.year, us.population / 10**6)
plt.plot(china.year, china.population / 10**6)
plt.legend(['United States', 'China'])
plt.xlabel('year')
plt.ylabel('population')
plt.show()


# %%
us.population


# %%
us.population / us.population.iloc[0] * 100


# %%
plt.plot(us.year, us.population / us.population.iloc[0] * 100)
plt.plot(china.year, china.population / china.population.iloc[0] * 100)
plt.legend(['United States', 'China'])
plt.xlabel('year')
plt.ylabel('population growth (first year = 100)')
plt.show()


# %%
# thanks for watching! :)


