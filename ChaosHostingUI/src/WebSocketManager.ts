import { hubUrl } from "./appConfig";
import * as signalR from "@microsoft/signalr";
import { CompleteGameState, PartialGameState } from "./ChaosApiClient";

export module WebSocketManager {
    let connection: signalR.HubConnection | undefined;

    export function setupConnection(
        newVoteOptions: (completeGameState: CompleteGameState) => any,
        voteUpdate: (partialGameState: PartialGameState) => any,
        roomCode: string
    ) {
        connection = new signalR.HubConnectionBuilder().withUrl(hubUrl).build();

        connection.on("newVoteOptions", newVoteOptions);
        connection.on("voteUpdate", voteUpdate);

        connection
            .start()
            .then(async () => {
                await registerAsVoter(roomCode);
            })
            .catch((err) => {
                console.log("Error when starting hub connection");
            });
    }

    export async function registerAsVoter(roomCode: string) {
        await connection?.send("registerAsVoter", roomCode);
    }
}
