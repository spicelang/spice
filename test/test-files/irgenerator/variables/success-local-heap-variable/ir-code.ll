; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [7 x i8] c"%d, %d\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %1 = alloca i32, align 4
  %i = alloca ptr, align 8
  %j = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  store i32 123, ptr %1, align 4
  %2 = call noundef ptr @_Z4sNewIiEPiRKi(ptr noundef %1)
  store ptr %2, ptr %i, align 8
  %3 = load ptr, ptr %i, align 8
  store ptr %3, ptr %j, align 8
  %4 = load ptr, ptr %i, align 8
  %5 = load i32, ptr %4, align 4
  %6 = load ptr, ptr %j, align 8
  %7 = load i32, ptr %6, align 4
  %8 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %5, i32 noundef %7)
  call void @_Z8sDeallocRPh(ptr %j)
  %9 = load i32, ptr %result, align 4
  ret i32 %9
}

declare ptr @_Z4sNewIiEPiRKi(ptr)

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

declare void @_Z8sDeallocRPh(ptr)

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
