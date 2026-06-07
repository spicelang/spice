; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [3 x i8] c"%d\00", align 4

define private noundef i32 @_Z4testv() {
  %result = alloca i32, align 4
  ret i32 12
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %fat.ptr = alloca { ptr, ptr, i64 }, align 8
  %testFct = alloca { ptr, ptr, i64 }, align 8
  %i = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store ptr @_Z4testv, ptr %fat.ptr, align 8
  %1 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr, i32 0, i32 1
  store ptr poison, ptr %1, align 8
  %2 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr, i32 0, i32 2
  store i64 0, ptr %2, align 8
  %3 = load { ptr, ptr, i64 }, ptr %fat.ptr, align 8
  store { ptr, ptr, i64 } %3, ptr %testFct, align 8
  %fct = load ptr, ptr %testFct, align 8
  %4 = call i32 %fct()
  store i32 %4, ptr %i, align 4
  %5 = load i32, ptr %i, align 4
  %6 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %5)
  %7 = load i32, ptr %result, align 4
  ret i32 %7
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
