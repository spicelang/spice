; ModuleID = 'source.spice'
source_filename = "source.spice"

declare ptr @malloc(i64 noundef)

declare void @free(ptr noundef)

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %address = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  %1 = call noundef ptr @malloc(i64 noundef 1)
  store ptr %1, ptr %address, align 8
  %2 = load ptr, ptr %address, align 8
  store i8 12, ptr %2, align 1
  %3 = load ptr, ptr %address, align 8
  call void @free(ptr noundef align 1 dereferenceable(1) %3)
  %4 = load i32, ptr %result, align 4
  ret i32 %4
}

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
