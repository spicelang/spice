; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Socket = type { i32, i16, %struct.NestedSocket }
%struct.NestedSocket = type { ptr, i64 }

@printf.str.0 = private unnamed_addr constant [17 x i8] c"Test string: %s\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [12 x i8] c"Socket: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %s = alloca %struct.Socket, align 8
  %n = alloca %struct.NestedSocket, align 8
  store i32 0, ptr %result, align 4
  %1 = call %struct.Socket @_Z16openServerSockett(i16 8080)
  store %struct.Socket %1, ptr %s, align 8
  %nested_addr = getelementptr inbounds %struct.Socket, ptr %s, i64 0, i32 2
  %2 = load %struct.NestedSocket, ptr %nested_addr, align 8
  store %struct.NestedSocket %2, ptr %n, align 8
  %testString_addr = getelementptr inbounds %struct.NestedSocket, ptr %n, i64 0, i32 0
  %3 = load ptr, ptr %testString_addr, align 8
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, ptr %3)
  %sock_addr = getelementptr inbounds %struct.Socket, ptr %s, i64 0, i32 0
  %5 = load i32, ptr %sock_addr, align 4
  %6 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %5)
  %7 = load i32, ptr %result, align 4
  ret i32 %7
}

declare %struct.Socket @_Z16openServerSockett(i16)

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
