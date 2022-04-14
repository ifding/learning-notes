
# Workflow scheduling


## Apache Airflow

The versatility of Airflow allows you to use it to schedule any type of workflows. It works best for pipelines:

- that change slowly
- related to the time interval
- scheduled on time

Apache Airflow can be used to schedule:

- ETL pipelines that extract data from multiple sources and run Spark jobs or any other data transformations
- Training machine learning models
- Report generation
- Backups and similar DevOps operations

Workflows are defined in Airflow by **DAGs** (Directed Acyclic Graphs) and are nothing more than a python file. A single DAG file may contain multiple DAG definitions, although it is recommended to keep one DAG per file.

```python
from airflow.models import DAG
from airflow.utils.dates import days_ago
from airflow.operators.dummy_operator import DummyOperator

with DAG(
  "etl_sales_daily",
  start_date=days_ago(1),
  schedule_interval=None,
) as dag:
   task_a = DummyOperator(task_id="task_a")
   task_b = DummyOperator(task_id="task_b")
   task_c = DummyOperator(task_id="task_c")
   task_d = DummyOperator(task_id="task_d")

   task_a >> [task_b, task_c]
   task_c >> task_d
```

In our example `task_b` and `task_c` are downstream of `task_a`. And respectively `task_a` is in upstream of both `task_b` and `task_c`. A common way of specifying a relation between tasks is using the `>>` operator which works for tasks and collection of tasks.