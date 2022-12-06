const std = @import("std");

fn isStartOfPacket(bytes: [*]const u8, packet_size: u8) bool {
    var index: u8 = 0;
    var found_lut: [26]bool = [_]bool{false} ** 26;

    var result = true;
    while (index < packet_size) : (index += 1) {
        const lut_index = bytes[index] - 'a';
        if (found_lut[lut_index] == true) {
            result = false;
            break;
        }

        found_lut[lut_index] = true;
    }

    return result;
}

fn findPacketStart(file_name: []const u8) !struct { packet_index: u32, message_index: u32 } {
    var file = try std.fs.cwd().openFile(file_name, .{});
    defer file.close();

    var packet_index: u32 = 0;
    var message_index: u32 = 0;

    var buf_reader = std.io.bufferedReader(file.reader());
    var in_stream = buf_reader.reader();
    var buf: [4096]u8 = undefined;
    while (try in_stream.readUntilDelimiterOrEof(&buf, '\n')) |line| {
        var slice = line[0..line.len];

        while (packet_index < slice.len) : (packet_index += 1) {
            if (isStartOfPacket(slice.ptr + packet_index, 4)) {
                // Start the search here for part 2
                message_index = packet_index;
                packet_index += 4;
                break;
            }
        }

        while (message_index < slice.len) : (message_index += 1) {
            if (isStartOfPacket(slice.ptr + message_index, 14)) {
                message_index += 14;
                break;
            }
        }
    }

    return .{ .packet_index = packet_index, .message_index = message_index };
}

pub fn main() !void {
    const indices = try findPacketStart("day-6-input.txt");

    const stdout = std.io.getStdOut().writer();

    try stdout.print("Start of packet: {d}\n", .{indices.packet_index});
    try stdout.print("Start of message: {d}\n", .{indices.message_index});
}

test "test_results" {
    const indices = try findPacketStart("day-6-input.txt");

    try std.testing.expect(indices.packet_index == 1175);
    try std.testing.expect(indices.message_index == 3217);
}
