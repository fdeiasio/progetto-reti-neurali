# Guide: finishing the project

The game works (`make run`). What is left is an agent that learns to land:
a Kohonen network (SOM) turns the continuous state into a discrete state,
and a Q-matrix picks the action for that state.

1. **Random starts.** A new `Lander` starts from a random position
   (`RandomLander`), so the agent does not learn a single trajectory.
2. **Actions.** List the possible inputs (nothing, thrust, left, right,
   thrust+left, thrust+right) in a table, so an action is an index.
3. **State vector.** Turn a `Lander` into a few normalised numbers
   (position relative to the landing zone, velocity, angle, fuel), each
   roughly in [0, 1] or [-1, 1].
4. **Kohonen network.** A grid of neurons with weight vectors the size of
   the state. Find the winner (closest neuron), move it and its neighbours
   towards the input; shrink learning rate and neighbourhood over time.
   The winner's index is the discrete state.
5. **Q-matrix.** One row per neuron, one column per action. Choose actions
   epsilon-greedy, update with Q-learning:
   `Q[s][a] += alpha * (r + gamma * max Q[s'] - Q[s][a])`.
6. **Reward.** Big positive reward for landing in the zone, big negative for
   crashing or leaving the screen, small shaping terms (closer to the zone,
   slower, more upright) in between.
7. **Training loop** (`app/train.cpp`, `make train`). Run many episodes:
   reset, step the physics with the agent's action, learn, until the
   episode ends. Decay epsilon. Print the success rate every N episodes.
8. **Save and load.** Write the SOM weights and the Q-matrix to a file, so
   the game can load them.
9. **Watch.** Add a mode to the game where the agent plays instead of the
   keyboard, to see the result.
10. **Tune.** Try SOM size, alpha, gamma, epsilon decay and the reward until
    the success rate is good; write down what you tried for the exam.

When step 7 exists, replace the `train` stub in the Makefile with a real
target that builds `app/train.cpp` with `src/core.cpp` and `src/som.cpp`
(no raylib window).
