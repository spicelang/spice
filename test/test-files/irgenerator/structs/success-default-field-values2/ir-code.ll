; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Test = type { i32, ptr }

@anon.string.0 = private unnamed_addr constant [5 x i8] c"test\00", align 4
@printf.str.0 = private unnamed_addr constant [9 x i8] c"Int: %d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [12 x i8] c"String: %s\0A\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %t = alloca %struct.Test, align 8
  store i32 0, ptr %result, align 4
  store %struct.Test { i32 12, ptr @anon.string.0 }, ptr %t, align 8
  %i.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 0
  %1 = load i32, ptr %i.addr, align 4
  %2 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %1)
  %s.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 1
  %3 = load ptr, ptr %s.addr, align 8
  %4 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr noundef %3)
  %5 = load i32, ptr %result, align 4
  ret i32 %5
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
