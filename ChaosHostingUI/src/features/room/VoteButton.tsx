import { castVote } from "./roomSlice";

interface VoteButtonProps {
    id: number;
    count: number;
    description: string;
    round: number;
    roomCode: string;
}

const VoteButton = ({
    id,
    count,
    description,
    round,
    roomCode,
}: VoteButtonProps) => {
    let btnColour = "btn-primary";

    switch (id) {
        case 0:
            btnColour = "btn-primary";
            break;
        case 1:
            btnColour = "btn-success";
            break;
        case 2:
            btnColour = "btn-danger";
            break;
        case 3:
            btnColour = "btn-dark";
            break;
    }

    return (
        <div className="col d-grid gap-1">
            <button
                className={`btn btn-lg ${btnColour}`}
                onClick={async () => {
                    await castVote(roomCode, round, id);
                }}
            >
                <span>{description}</span>
                <br />
                <span>Votes: {count}</span>
            </button>
        </div>
    );
};

export default VoteButton;
