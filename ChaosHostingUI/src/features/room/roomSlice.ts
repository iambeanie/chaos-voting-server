import { createSlice, Dispatch, PayloadAction } from "@reduxjs/toolkit";
import {
    CompleteGameState,
    FullVoteOptionState,
    PartialGameState,
    PartialVoteState,
} from "../../ChaosApiClient";
import { ChaosApiClient } from "../../ChaosApiClient";
import { apiUrl } from "../../appConfig";

let ChaosApi = new ChaosApiClient(apiUrl);

export interface RoomState {
    gameState: GameState;
}

export interface GameState {
    previousResultDescription: string | undefined;
    currentRound: number;
    gameVotesState: GameVoteState[];
}

export interface GameVoteState {
    id: number;
    count: number;
    description: string;
}

export interface VoteUpdate {
    id: number | undefined;
    count: number | undefined;
}

export const initialState: RoomState = {
    gameState: {
        previousResultDescription: undefined,
        currentRound: 0,
        gameVotesState: [],
    },
};

export const roomSlice = createSlice({
    name: "room",
    initialState,
    reducers: {
        updateCompleteGameState: (state, action: PayloadAction<GameState>) => {
            state.gameState.currentRound = action.payload.currentRound;
            state.gameState.gameVotesState = action.payload.gameVotesState;
            state.gameState.previousResultDescription =
                action.payload.previousResultDescription;
        },
        updatePartialGameState: (
            state,
            action: PayloadAction<VoteUpdate[]>
        ) => {
            const newGameVoteState: GameVoteState[] = state.gameState.gameVotesState.map(
                (gvs: GameVoteState) => {
                    const matchingVoteUpdate:
                        | VoteUpdate
                        | undefined = action.payload.find((vu: VoteUpdate) => {
                        return vu.id === gvs.id;
                    });

                    return {
                        ...gvs,
                        count: matchingVoteUpdate?.count ?? gvs.count,
                    };
                }
            );

            state.gameState.gameVotesState = newGameVoteState;
        },
    },
});

export const updateCompleteGameStateFromSocketUpdate = (
    completeGameState: CompleteGameState
) => {
    return async (dispatch: Dispatch) => {
        const gameState: GameState = {
            currentRound:
                completeGameState.currentRound ??
                initialState.gameState.currentRound,
            previousResultDescription:
                completeGameState.previousResultDescription ??
                initialState.gameState.previousResultDescription,
            gameVotesState:
                completeGameState.voteOptionsState?.map(
                    (vos: FullVoteOptionState): GameVoteState => {
                        return {
                            count: vos.count ?? 0,
                            description: vos.description ?? "",
                            id: vos.id ?? -1,
                        };
                    }
                ) ?? initialState.gameState.gameVotesState,
        };

        return dispatch(roomSlice.actions.updateCompleteGameState(gameState));
    };
};

export const updatePartialGameStateFromSocketUpdate = (
    partialGameState: PartialGameState
) => {
    return async (dispatch: Dispatch) => {
        const voteUpdate:
            | VoteUpdate[]
            | undefined = partialGameState?.partialVoteStates?.map(
            (pvs: PartialVoteState): VoteUpdate => {
                return {
                    count: pvs.count,
                    id: pvs.id,
                };
            }
        );

        if (!voteUpdate) return;

        return dispatch(roomSlice.actions.updatePartialGameState(voteUpdate));
    };
};

export const createGameVoteStatesFromApiModel = (
    partialGameState: PartialGameState,
    currentGameVoteStates: GameVoteState[]
) => {
    const newVoteStates: GameVoteState[] = [];

    currentGameVoteStates.forEach((cgvs: GameVoteState) => {
        const matchingPartialVoteState:
            | PartialVoteState
            | undefined = partialGameState.partialVoteStates!.find(
            (pvs: PartialVoteState) => pvs.id === cgvs.id
        );

        if (matchingPartialVoteState) {
            newVoteStates.push({
                ...cgvs,
                count: matchingPartialVoteState.count ?? 0,
            });
        } else {
            newVoteStates.push({
                ...cgvs,
            });
        }
    });

    return newVoteStates;
};

export const loadInitialGameState = (roomCode: string) => {
    return async (dispatch: Dispatch) => {
        await ChaosApi.getCompleteGameState(roomCode)
            .then((completeGameState: CompleteGameState) => {
                return dispatch(
                    roomSlice.actions.updateCompleteGameState({
                        currentRound:
                            completeGameState.currentRound ??
                            initialState.gameState.currentRound,
                        previousResultDescription:
                            completeGameState.previousResultDescription ??
                            initialState.gameState.previousResultDescription,
                        gameVotesState:
                            completeGameState.voteOptionsState?.map(
                                (vos: FullVoteOptionState): GameVoteState => {
                                    return {
                                        count: vos.count ?? 0,
                                        description: vos.description ?? "",
                                        id: vos.id ?? -1,
                                    };
                                }
                            ) ?? initialState.gameState.gameVotesState,
                    })
                );
            })
            .catch((error) => {
                console.log("error retrieving initial game state: " + error);
            });
    };
};

export const castVote = async (
    roomCode: string,
    round: number,
    optionId: number
) => {
    try {
        await ChaosApi.vote(roomCode, round, optionId);
    } catch (e) {
        console.log(e);
    }
};

export default roomSlice.reducer;
