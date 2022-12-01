const std = @import("std");

pub fn getMaxCalories( file_name: []const u8) anyerror!i32
{
    var file = try std.fs.cwd().openFile(file_name, .{ });
    defer file.close();

    var buf_reader = std.io.bufferedReader(file.reader());
    var in_stream = buf_reader.reader();


    var buf: [256]u8 = undefined;
    var calorie_count: i32 = 0;
    var max_calorie_count: i32 = -1;
    while (try in_stream.readUntilDelimiterOrEof(&buf, '\n')) |line|
    {
        if( std.mem.eql(u8, line, "") )
        {
            if (calorie_count > max_calorie_count)
            {
                max_calorie_count = calorie_count;
            }

            calorie_count = 0;
        }
        else
        {
            const calories = try std.fmt.parseInt(i32, line, 10);
            calorie_count += calories;
        }
    }

    return max_calorie_count;
}

pub fn getTopThreeCalories( file_name: []const u8) anyerror!i32
{
    var file = try std.fs.cwd().openFile(file_name, .{ });
    defer file.close();

    var buf_reader = std.io.bufferedReader(file.reader());
    var in_stream = buf_reader.reader();

    var buf: [256]u8 = undefined;
    var calorie_count: i32 = 0;
    var max_calorie_counts: [3]i32 = .{-1, -1, -1};
    var mindex: u8 = 0;
    while (try in_stream.readUntilDelimiterOrEof(&buf, '\n')) |line|
    {
        if( std.mem.eql(u8, line, "") )
        {
            var i: u8 = 0;
            if(calorie_count > max_calorie_counts[mindex])
            {
                max_calorie_counts[mindex] = calorie_count;

                while(i < 3) : (i += 1)
                {
                    if (max_calorie_counts[i] < max_calorie_counts[mindex])
                    {
                        mindex = i;
                    }
                }
            }
            calorie_count = 0;
        }
        else
        {
            const calories = try std.fmt.parseInt(i32, line, 10);
            calorie_count += calories;
        }
    }

    return max_calorie_counts[0] + max_calorie_counts[1] + max_calorie_counts[2];
}


pub fn main() anyerror!void
{
    const max_calorie_count = try getMaxCalories("day-1-input.txt");

    std.debug.print("Highest calorie count: {d}\n", .{max_calorie_count});

    const top_three_sum = try getTopThreeCalories("day-1-input.txt");

    std.debug.print("Sum of top three counts: {d}\n", .{top_three_sum});
}

test "part_1"
{
    const max_calorie_count = try getMaxCalories("day-1-input.txt");

    try std.testing.expect (max_calorie_count == 70720);
}

test "part_2"
{
    const top_three_sum = try getTopThreeCalories("day-1-input.txt");

    try std.testing.expect (top_three_sum == 207148);
}
