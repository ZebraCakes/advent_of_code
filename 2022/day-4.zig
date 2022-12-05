const std = @import("std");

const Job = struct {
    start: u32,
    end: u32,
};

fn findOverlaps(file_name: []const u8) !struct { full_overlap_count: u32, partial_overlap_count: u32 } {
    var file = try std.fs.cwd().openFile(file_name, .{});
    defer file.close();

    var buf_reader = std.io.bufferedReader(file.reader());
    var in_stream = buf_reader.reader();

    var full_overlap_count: u32 = 0;
    var partial_overlap_count: u32 = 0;
    var buf: [256]u8 = undefined;
    while (try in_stream.readUntilDelimiterOrEof(&buf, '\n')) |line| {
        const delim_index = std.mem.indexOf(u8, line, ",").?;
        const job_1_input = line[0..delim_index];
        const job_2_input = line[delim_index + 1 .. line.len];

        var job_delim_index = std.mem.indexOf(u8, job_1_input, "-").?;
        const job_1: Job = .{ .start = std.fmt.parseInt(u32, job_1_input[0..job_delim_index], 10) catch unreachable, .end = std.fmt.parseInt(u32, job_1_input[job_delim_index + 1 .. job_1_input.len], 10) catch unreachable };

        job_delim_index = std.mem.indexOf(u8, job_2_input, "-").?;
        const job_2: Job = .{ .start = std.fmt.parseInt(u32, job_2_input[0..job_delim_index], 10) catch unreachable, .end = std.fmt.parseInt(u32, job_2_input[job_delim_index + 1 .. job_2_input.len], 10) catch unreachable };

        if ((job_1.start >= job_2.start and job_1.end <= job_2.end) or
            (job_1.start <= job_2.start and job_1.end >= job_2.end))
        {
            full_overlap_count += 1;
        }

        const min_job = if (job_1.start < job_2.start) &job_1 else &job_2;
        const max_job = if (job_1.start >= job_2.start) &job_1 else &job_2;
        if (max_job.start <= min_job.end) {
            partial_overlap_count += 1;
        }
    }

    return .{ .full_overlap_count = full_overlap_count, .partial_overlap_count = partial_overlap_count };
}
pub fn main() !void {
    const overlap_counts = try findOverlaps("day-4-input.txt");

    const stdout = std.io.getStdOut().writer();

    try stdout.print("# complete overlaps: {}\n", .{overlap_counts.full_overlap_count});
    try stdout.print("# partial overlaps: {}\n", .{overlap_counts.partial_overlap_count});
}

test "test_results" {
    const overlap_counts = try findOverlaps("day-4-input.txt");

    try std.testing.expect(overlap_counts.full_overlap_count == 588);
    try std.testing.expect(overlap_counts.partial_overlap_count == 911);
}
