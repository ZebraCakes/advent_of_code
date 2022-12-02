const std = @import("std");

const Scores = struct { fake_score: i32, real_score: i32 };

pub fn processScore(file_name: []const u8) anyerror!Scores {
    var file = try std.fs.cwd().openFile(file_name, .{});
    defer file.close();

    var buf_reader = std.io.bufferedReader(file.reader());
    var in_stream = buf_reader.reader();

    var scores: Scores = .{ .fake_score = 0, .real_score = 0 };
    var buf: [16]u8 = undefined;
    while (try in_stream.readUntilDelimiterOrEof(&buf, '\n')) |line| {
        // normalize these to 0-2
        const opponent_hand = line[0] - 'A';
        const my_hand = line[2] - 'X';
        const win_result = my_hand;

        const win_matrix = [3][3]u8{ [_]u8{ 3, 0, 6 }, [_]u8{ 6, 3, 0 }, [_]u8{ 0, 6, 3 } };
        const hand_matrix = [3][3]u8{ [_]u8{ 3, 1, 2 }, [_]u8{ 1, 2, 3 }, [_]u8{ 2, 3, 1 } };

        scores.fake_score += win_matrix[my_hand][opponent_hand] + (my_hand + 1);

        scores.real_score += (win_result * 3) + hand_matrix[win_result][opponent_hand];
    }

    return scores;
}

pub fn main() anyerror!void {
    const scores = try processScore("day-2-input.txt");

    const stdout = std.io.getStdOut().writer();
    try stdout.print("Fake score: {}\n", .{scores.fake_score});
    try stdout.print("Real score: {}\n", .{scores.real_score});
}

test "test_results" {
    const scores = try processScore("day-2-input.txt");

    try std.testing.expect(scores.fake_score == 15523);
    try std.testing.expect(scores.real_score == 15702);
}
