const std = @import("std");

fn computeTopsOfStacks2(file_name: []const u8) ![]const u8 {
    var file = try std.fs.cwd().openFile(file_name, .{});
    defer file.close();

    var buf_reader = std.io.bufferedReader(file.reader());
    var in_stream = buf_reader.reader();

    const allocator = std.heap.page_allocator;
    var stacks = [1]std.ArrayListUnmanaged(u8){.{}} ** 9;
    defer for (stacks) |*stack| {
        stack.clearAndFree(allocator);
    };

    var cur_stack: u32 = 0;

    var parsingStacks: bool = true;
    var buf: [256]u8 = undefined;
    while (try in_stream.readUntilDelimiterOrEof(&buf, '\n')) |line| {
        if (parsingStacks) {
            if (std.mem.eql(u8, line, "")) {
                parsingStacks = false;
                continue;
            }

            var splits = std.mem.split(u8, line, ",");
            while (splits.next()) |split| {
                try stacks[cur_stack].append(allocator, split[0]);
            }

            cur_stack += 1;
        } else {
            var tokens = std.mem.split(u8, line, " ");

            // skip "move"
            _ = tokens.next();
            var count = try std.fmt.parseInt(u32, tokens.next().?, 10);

            // skip "from"
            _ = tokens.next();
            const src_index = (try std.fmt.parseInt(u32, tokens.next().?, 10)) - 1;

            // skip "to"
            _ = tokens.next();
            const dst_index = (try std.fmt.parseInt(u32, tokens.next().?, 10)) - 1;

            var temp_stack = std.ArrayListUnmanaged(u8){};
            defer temp_stack.clearAndFree(allocator);

            while (count > 0) : (count -= 1) {
                var char = stacks[src_index].pop();
                try temp_stack.append(allocator, char);
            }

            while (temp_stack.items.len > 0) {
                var char = temp_stack.pop();
                try stacks[dst_index].append(allocator, char);
            }
        }
    }

    const message = try allocator.alloc(u8, 9);

    var index: u32 = 0;
    while (index < 9) : (index += 1) {
        var char = stacks[index].pop();
        message[index] = char;
    }

    return message;
}

fn computeTopsOfStacks(file_name: []const u8) ![]const u8 {
    var file = try std.fs.cwd().openFile(file_name, .{});
    defer file.close();

    var buf_reader = std.io.bufferedReader(file.reader());
    var in_stream = buf_reader.reader();

    const allocator = std.heap.page_allocator;
    var stacks = [1]std.ArrayListUnmanaged(u8){.{}} ** 9;
    defer for (stacks) |*stack| {
        stack.clearAndFree(allocator);
    };

    var cur_stack: u32 = 0;

    var parsingStacks: bool = true;
    var buf: [256]u8 = undefined;
    while (try in_stream.readUntilDelimiterOrEof(&buf, '\n')) |line| {
        if (parsingStacks) {
            if (std.mem.eql(u8, line, "")) {
                parsingStacks = false;
                continue;
            }

            var splits = std.mem.split(u8, line, ",");
            while (splits.next()) |split| {
                try stacks[cur_stack].append(allocator, split[0]);
            }

            cur_stack += 1;
        } else {
            var tokens = std.mem.split(u8, line, " ");

            // skip "move"
            _ = tokens.next();
            var count = try std.fmt.parseInt(u32, tokens.next().?, 10);

            // skip "from"
            _ = tokens.next();
            const src_index = (try std.fmt.parseInt(u32, tokens.next().?, 10)) - 1;

            // skip "to"
            _ = tokens.next();
            const dst_index = (try std.fmt.parseInt(u32, tokens.next().?, 10)) - 1;

            while (count > 0) : (count -= 1) {
                var char = stacks[src_index].pop();
                try stacks[dst_index].append(allocator, char);
            }
        }
    }

    const message = try allocator.alloc(u8, 9);

    var index: u32 = 0;
    while (index < 9) : (index += 1) {
        var char = stacks[index].pop();
        message[index] = char;
    }

    return message;
}

pub fn main() !void {
    const message = try computeTopsOfStacks("day-5-input.txt");
    const stdout = std.io.getStdOut().writer();

    try stdout.print("Message: {s}\n", .{message});

    const message_2 = try computeTopsOfStacks2("day-5-input.txt");
    try stdout.print("Message 2: {s}\n", .{message_2});
}

test "test_results" {
    const message = try computeTopsOfStacks("day-5-input.txt");
    try std.testing.expect(std.mem.eql(u8, message, "JDTMRWCQJ"));

    const message_2 = try computeTopsOfStacks2("day-5-input.txt");
    try std.testing.expect(std.mem.eql(u8, message_2, "VHJDDCWRD"));
}
