; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.TreeNode = type { ptr, i32 }

@printf.str.0 = private unnamed_addr constant [21 x i8] c"Root node number: %d\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %rootNode = alloca %struct.TreeNode, align 8
  %_childNode1 = alloca %struct.TreeNode, align 8
  %childNode2 = alloca %struct.TreeNode, align 8
  %childNode21 = alloca %struct.TreeNode, align 8
  %curNode = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  store %struct.TreeNode zeroinitializer, ptr %rootNode, align 8
  store ptr %rootNode, ptr %_childNode1, align 8
  %1 = getelementptr inbounds nuw %struct.TreeNode, ptr %_childNode1, i32 0, i32 1
  store i32 1, ptr %1, align 4
  store ptr %rootNode, ptr %childNode2, align 8
  %2 = getelementptr inbounds nuw %struct.TreeNode, ptr %childNode2, i32 0, i32 1
  store i32 2, ptr %2, align 4
  store ptr %childNode2, ptr %childNode21, align 8
  %3 = getelementptr inbounds nuw %struct.TreeNode, ptr %childNode21, i32 0, i32 1
  store i32 3, ptr %3, align 4
  store ptr %childNode21, ptr %curNode, align 8
  br label %while.head.L21

while.head.L21:                                   ; preds = %while.body.L21, %0
  %4 = load ptr, ptr %curNode, align 8
  %parent.addr = getelementptr inbounds %struct.TreeNode, ptr %4, i64 0, i32 0
  %5 = load ptr, ptr %parent.addr, align 8
  %6 = icmp ne ptr %5, null
  br i1 %6, label %while.body.L21, label %while.exit.L21

while.body.L21:                                   ; preds = %while.head.L21
  %7 = load ptr, ptr %curNode, align 8
  %parent.addr1 = getelementptr inbounds %struct.TreeNode, ptr %7, i64 0, i32 0
  %8 = load ptr, ptr %parent.addr1, align 8
  store ptr %8, ptr %curNode, align 8
  br label %while.head.L21

while.exit.L21:                                   ; preds = %while.head.L21
  %9 = load ptr, ptr %curNode, align 8
  %value.addr = getelementptr inbounds %struct.TreeNode, ptr %9, i64 0, i32 1
  %10 = load i32, ptr %value.addr, align 4
  %11 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %10)
  %12 = load i32, ptr %result, align 4
  ret i32 %12
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
