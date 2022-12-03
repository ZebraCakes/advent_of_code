const std = @import("std");

fn getPriorityIndex(character: u8) u8 {
    return if (character >= 'a') character - 'a' else character - 'A' + 26;
}

fn processBags(file_name: []const u8) !struct { p1_sum: u32, p2_sum: u32 } {
    var file = try std.fs.cwd().openFile(file_name, .{});
    defer file.close();

    var buf_reader = std.io.bufferedReader(file.reader());
    var in_stream = buf_reader.reader();

    var p1_sum: u32 = 0;
    var p2_sum: u32 = 0;

    var p2_array: [2][52]bool = [2][52]bool{ [_]bool{false} ** 52, [_]bool{false} ** 52 };
    var elf_num: u32 = 0;

    var buf: [256]u8 = undefined;
    while (try in_stream.readUntilDelimiterOrEof(&buf, '\n')) |line| {
        // Part 1
        const sack_1_slice = line[0 .. line.len / 2];
        const sack_2_slice = line[line.len / 2 .. line.len];

        var counts: [52]bool = [_]bool{false} ** 52;
        for (sack_1_slice) |character| {
            counts[getPriorityIndex(character)] = true;
        }

        for (sack_2_slice) |character| {
            const index = getPriorityIndex(character);

            if (counts[index] == true) {
                p1_sum += index + 1;
                break;
            }
        }

        // Part 2
        const elf_index = elf_num % 3;
        if (elf_index < 2) {
            for (line) |character| {
                p2_array[elf_index][getPriorityIndex(character)] = true;
            }
        } else {
            for (line) |character| {
                const index = getPriorityIndex(character);
                if (p2_array[0][index] == true and
                    p2_array[1][index] == true)
                {
                    p2_sum += index + 1;
                    break;
                }
            }

            // Done with a trio, reset arrays.
            p2_array = [2][52]bool{ [_]bool{false} ** 52, [_]bool{false} ** 52 };
        }

        elf_num += 1;
    }

    return .{ .p1_sum = p1_sum, .p2_sum = p2_sum };
}

pub fn main() !void {
    const priorities = try processBags("day-3-input.txt");

    const stdout = std.io.getStdOut().writer();
    try stdout.print("Sum of priorities: {}\n", .{priorities.p1_sum});
    try stdout.print("Sum of badges: {}\n", .{priorities.p2_sum});
}

test "test_results" {
    const priorities = try processBags("day-3-input.txt");

    try std.testing.expect(priorities.p1_sum == 8349);
    try std.testing.expect(priorities.p2_sum == 2681);
}
