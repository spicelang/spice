; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [11 x i8] c"Result: %d\00", align 4

define private i1 @_Z10condition1v() {
  %result = alloca i1, align 1
  ret i1 false
}

define private i1 @_Z10condition2v() {
  %result = alloca i1, align 1
  ret i1 true
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %1 = call i1 @_Z10condition1v()
  br i1 %1, label %land.1.L10C26, label %land.exit.L10C26

land.1.L10C26:                                    ; preds = %0
  %2 = call i1 @_Z10condition2v()
  br label %land.exit.L10C26

land.exit.L10C26:                                 ; preds = %land.1.L10C26, %0
  %land_phi = phi i1 [ %1, %0 ], [ %2, %land.1.L10C26 ]
  %3 = select i1 %land_phi, i32 2, i32 3
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %3)
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
