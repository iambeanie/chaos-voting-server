import { useEffect } from "react";
import { useSelector, useDispatch } from "react-redux";
import {
    GameVoteState,
    loadInitialGameState,
    updateCompleteGameStateFromSocketUpdate,
    updatePartialGameStateFromSocketUpdate,
} from "./roomSlice";
import VoteButton from "./VoteButton";
import { CompleteGameState, PartialGameState } from "../../ChaosApiClient";
import { WebSocketManager } from "../../WebSocketManager";
import { RootState } from "../../app/store";
import { useParams } from "react-router-dom";

interface RoomParams {
    roomCode: string;
}

export function Room() {
    const { roomCode } = useParams<RoomParams>();

    const currentRound = useSelector(
        (state: RootState) => state.room.gameState.currentRound,
        (left: number, right: number) => left === right
    );

    const gameVotesState: GameVoteState[] = useSelector(
        (state: RootState) => state.room.gameState.gameVotesState,
        (left: GameVoteState[], right: GameVoteState[]) => {
            if (left !== right) return false;
            if (left.length !== right.length) return false;

            for (let i = 0; i < left.length; i++) {
                if (left[i].count !== right[i].count) {
                    return false;
                }
            }

            return true;
        }
    );

    const previousResultDescription = useSelector(
        (state: RootState) => state.room.gameState.previousResultDescription,
        (left: string | undefined, right: string | undefined) => left === right
    );

    const dispatch = useDispatch();

    const handleNewVoteOptions = (completeGameState: CompleteGameState) => {
        if (completeGameState == null) return;
        dispatch(updateCompleteGameStateFromSocketUpdate(completeGameState));
    };

    const handleVoteUpdate = (partialGameState: PartialGameState) => {
        if (partialGameState == null) return;
        dispatch(updatePartialGameStateFromSocketUpdate(partialGameState));
    };

    useEffect(() => {
        WebSocketManager.setupConnection(
            handleNewVoteOptions,
            handleVoteUpdate,
            roomCode
        );

        dispatch(loadInitialGameState(roomCode));
    }, []);

    return (
        <>
            {previousResultDescription && (
                <p className="lead text-center">
                    Previous vote: {previousResultDescription}
                </p>
            )}

            {(!gameVotesState || gameVotesState.length === 0) && (
                <p className="lead text-center">
                    Waiting for initial voting options
                </p>
            )}

            {gameVotesState && gameVotesState.length > 0 && (
                <>
                    <hr />
                    <div className="container">
                        <div className="row row-cols-2 gy-2">
                            {gameVotesState.map(
                                (gameVoteState: GameVoteState) => {
                                    return (
                                        <VoteButton
                                            key={gameVoteState.id}
                                            count={gameVoteState.count}
                                            description={
                                                gameVoteState.description
                                            }
                                            round={currentRound}
                                            id={gameVoteState.id}
                                            roomCode={roomCode}
                                        />
                                    );
                                }
                            )}
                        </div>
                    </div>
                </>
            )}
        </>
    );
}
