const std = @import("std");

const CalorieCounts = struct { max_calorie_count: i32, top_3_calorie_sum: i32 };

pub fn processCalories(file_name: []const u8) anyerror!CalorieCounts {
    var file = try std.fs.cwd().openFile(file_name, .{});
    defer file.close();

    var buf_reader = std.io.bufferedReader(file.reader());
    var in_stream = buf_reader.reader();

    var calorie_count: i32 = 0;
    var mindex: u8 = 0;
    var top_3_calorie_counts: [3]i32 = .{ -1, -1, -1 };

    // returned values
    var counts: CalorieCounts = .{ .max_calorie_count = -1, .top_3_calorie_sum = 0 };

    var buf: [256]u8 = undefined;
    while (try in_stream.readUntilDelimiterOrEof(&buf, '\n')) |line| {
        if (std.mem.eql(u8, line, "")) {
            if (calorie_count > counts.max_calorie_count) {
                counts.max_calorie_count = calorie_count;
            }

            var i: u8 = 0;
            if (calorie_count > top_3_calorie_counts[mindex]) {
                top_3_calorie_counts[mindex] = calorie_count;

                while (i < 3) : (i += 1) {
                    if (top_3_calorie_counts[i] < top_3_calorie_counts[mindex]) {
                        mindex = i;
                    }
                }
            }

            calorie_count = 0;
        } else {
            const calories = try std.fmt.parseInt(i32, line, 10);
            calorie_count += calories;
        }
    }

    counts.top_3_calorie_sum = top_3_calorie_counts[0] + top_3_calorie_counts[1] + top_3_calorie_counts[2];
    return counts;
}

pub fn main() anyerror!void {
    const counts = try processCalories("day-1-input.txt");

    std.debug.print("Highest calorie count: {d}\n", .{counts.max_calorie_count});
    std.debug.print("Sum of top three counts: {d}\n", .{counts.top_3_calorie_sum});
}

test "test_results" {
    const counts = try processCalories("day-1-input.txt");

    try std.testing.expect(counts.max_calorie_count == 70720);
    try std.testing.expect(counts.top_3_calorie_sum == 207148);
}
