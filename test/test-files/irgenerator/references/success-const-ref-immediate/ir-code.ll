; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Struct = type { ptr }

@printf.str.0 = private unnamed_addr constant [16 x i8] c"Field value: %d\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %str = alloca %struct.Struct, align 8
  %1 = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store i32 123, ptr %1, align 4
  store ptr %1, ptr %str, align 8
  %ref.addr = getelementptr inbounds %struct.Struct, ptr %str, i64 0, i32 0
  %2 = load ptr, ptr %ref.addr, align 8
  %3 = load i32, ptr %2, align 4
  %4 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %3)
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
