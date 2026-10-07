; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Socket = type { i32, i16, %struct.NestedSocket }
%struct.NestedSocket = type { ptr, i64 }

@printf.str.0 = private unnamed_addr constant [17 x i8] c"Test string: %s\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [12 x i8] c"Socket: %d\0A\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %s = alloca %struct.Socket, align 8
  %n = alloca %struct.NestedSocket, align 8
  call void @_Z16openServerSockett(ptr dead_on_unwind writable sret(%struct.Socket) align 8 %s, i16 noundef zeroext 8080)
  %nested.addr = getelementptr inbounds %struct.Socket, ptr %s, i64 0, i32 2
  call void @llvm.memcpy.p0.p0.i64(ptr %n, ptr %nested.addr, i64 16, i1 false)
  %testString.addr = getelementptr inbounds %struct.NestedSocket, ptr %n, i64 0, i32 0
  %1 = load ptr, ptr %testString.addr, align 8
  %2 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, ptr noundef %1)
  %sock.addr = getelementptr inbounds %struct.Socket, ptr %s, i64 0, i32 0
  %3 = load i32, ptr %sock.addr, align 4
  %4 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 noundef %3)
  ret i32 0
}

declare void @_Z16openServerSockett(ptr dead_on_unwind noalias writable sret(%struct.Socket) align 8, i16)

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #1

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev [host] (https://github.com/spicelang/spice)"}
