from flask import Flask, request, jsonify
from flask_sqlalchemy import SQLAlchemy
from sqlalchemy import func
from flask_cors import CORS

app = Flask(__name__)
CORS(app, resources={r"/*": {"origins": "*"}})
app.config['SQLALCHEMY_DATABASE_URI'] = 'mysql+pymysql://root:Faya2020@127.0.0.1:3306/lab3'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False

db = SQLAlchemy(app)

def mapper(func, data):
    if type(data) != int:
        return None
    if func == "sex":
        return "男" if data == 1 else "女"
    elif func == "teacher_ranks":
        return ["博士后", "助教", "讲师", "副教授", "特任教授", "教授", "助理研究员", "特任副研究员", "副研究员", "特任研究员", "研究员"][data - 1]
    elif func == "paper_types":
        return ["full paper", "short paper", "poster paper", "demo paper"][data - 1]
    elif func == "paper_level":
        return ["CCF-A", "CCF-B", "CCF-C", "中文 CCF-A", "中文 CCF-B", "无级别"][data - 1]
    elif func == "project_types":
        return ["国家级项目", "省部级项目", "市厅级项目", "企业合作项目", "其它类型项目"][data - 1]
    elif func == "course_term":
        return ["春", "夏", "秋"][data - 1]
    elif func == "course_property":
        return "本科生课程" if data == 1 else "研究生课程"
    elif func == "project_types_remap":
        return ["国家级项目", "省部级项目", "市厅级项目", "企业合作项目", "其它类型项目"].index(data) + 1
    else:
        return None

class Paper(db.Model):
    __tablename__ = 'paper'
    id = db.Column(db.Integer, primary_key=True, autoincrement=True)
    title = db.Column(db.String(256))
    source = db.Column(db.String(256))
    date = db.Column(db.Date)
    types = db.Column(db.Integer)
    level = db.Column(db.Integer)
    __table_args__ = (
        db.CheckConstraint('types in (1, 2, 3, 4)'),
        db.CheckConstraint('level in (1, 2, 3, 4, 5, 6)'),
    )

    def __init__(self, info: dict):
        self.title = info['title']
        self.source = info['source']
        self.date = info['date']
        self.types = info['types']
        self.level = info['level']

class Teacher(db.Model):
    __tablename__ = 'teacher'
    id = db.Column(db.String(5), primary_key=True)
    name = db.Column(db.String(256))
    sex = db.Column(db.Integer)
    ranks = db.Column(db.Integer)
    __table_args__ = (
        db.CheckConstraint('sex in (1, 2)'),
        db.CheckConstraint('ranks in (1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11)'),
    )

    def __init__(self, info: dict):
        self.id = info['id']
        self.name = info['name']
        self.sex = info['sex']
        self.ranks = info['ranks']

class Course(db.Model):
    __tablename__ = 'course'
    id = db.Column(db.String(256), primary_key=True)
    name = db.Column(db.String(256))
    hours = db.Column(db.Integer)
    property = db.Column(db.Integer)
    __table_args__ = (
        db.CheckConstraint('property in (1, 2)'),
    )

    def __init__(self, info: dict):
        self.id = info['id']
        self.name = info['name']
        self.hours = info['hours']
        self.property = info['property']

class Project(db.Model):
    __tablename__ = 'project'
    id = db.Column(db.String(256), primary_key=True)
    name = db.Column(db.String(256))
    source = db.Column(db.String(256))
    types = db.Column(db.Integer)
    money = db.Column(db.Float)
    start_year = db.Column(db.Integer)
    end_year = db.Column(db.Integer)
    __table_args__ = (
        db.CheckConstraint('types in (1, 2, 3, 4, 5)'),
    )

    def __init__(self, info: dict):
        self.id = info['id']
        self.name = info['name']
        self.source = info['source']
        self.types = info['types']
        self.money = info['money']
        self.start_year = info['start_year']
        self.end_year = info['end_year']

class Publicate(db.Model):
    __tablename__ = 'publicate'
    teacher_id = db.Column(db.String(5), db.ForeignKey('teacher.id'), primary_key=True)
    paper_id = db.Column(db.Integer, db.ForeignKey('paper.id'), primary_key=True)
    orders = db.Column(db.Integer)
    is_ca = db.Column(db.Boolean)
    __table_args__ = (
        db.CheckConstraint('orders >= 1', name="check_publicate_orders"),
    )

    def __init__(self, info: dict):
        self.teacher_id = info['teacher_id']
        self.paper_id = info['paper_id']
        self.orders = info['orders']
        self.is_ca = info['is_ca']

class Teach(db.Model):
    __tablename__ = 'teach'
    teacher_id = db.Column(db.String(5), db.ForeignKey('teacher.id'), primary_key=True)
    course_id = db.Column(db.String(256), db.ForeignKey('course.id'), primary_key=True)
    year = db.Column(db.Integer)
    term = db.Column(db.Integer)
    hours = db.Column(db.Integer)
    __table_args__ = (
        db.CheckConstraint('year >= 2000'),
        db.CheckConstraint('term in (1, 2, 3)'),
    )

    def __init__(self, info: dict):
        self.teacher_id = info['teacher_id']
        self.course_id = info['course_id']
        self.year = info['year']
        self.term = info['term']
        self.hours = info['hours']
    
class Undertake(db.Model):
    __tablename__ = 'undertake'
    teacher_id = db.Column(db.String(5), db.ForeignKey('teacher.id'), primary_key=True)
    project_id = db.Column(db.String(256), db.ForeignKey('project.id'), primary_key=True)
    orders = db.Column(db.Integer)
    undertake_money = db.Column(db.Float)
    __table_args__ = (
        db.CheckConstraint('orders >= 1', name='check_undertake_orders'),
    )

    def __init__(self, info: dict):
        self.teacher_id = info['teacher_id']
        self.project_id = info['project_id']
        self.orders = info['orders']
        self.undertake_money = info['undertake_money']

# 0.查询所有老师的详细信息
@app.route('/get_all_teachers_detail', methods=['GET'])
def get_all_teachers_detail():
    try:
        teachers = Teacher.query.all()
        return jsonify([{
                "id": teacher.id,
                "name": teacher.name,
                "sex": mapper("sex", teacher.sex),
                "ranks": mapper("teacher_ranks", teacher.ranks)
            } for teacher in teachers])
    except Exception as e:
        print(e)
        return jsonify([])
    
# 0.查询所有课程的详细信息
@app.route('/get_all_courses_detail', methods=['GET'])
def get_all_courses_detail():
    try:
        courses = Course.query.all()
        return jsonify([{
                "id": course.id,
                "name": course.name,
                "hours": course.hours,
                "course_property": mapper("course_property", course.property),
            } for course in courses])
    except Exception as e:
        print(e)
        return jsonify([])

# 0.查询所有论文的详细信息
#   此处作者信息是一个格式化的字符串，不是json
@app.route('/get_all_papers_detail', methods=['GET'])
def get_all_papers_detail():
    try:
        papers = Paper.query.all()
        papers_return = []
        for paper in papers:
            author_dict = {}
            # 查询论文作者
            authors = Publicate.query.filter_by(paper_id=paper.id).all()
            for author in authors:
                teacher_name = Teacher.query.filter_by(id=author.teacher_id).first().name
                author_dict[f'{teacher_name}({"通讯作者, " if author.is_ca else ""}{author.teacher_id})'] = int(author.orders)
            # 对作者按照orders排序
            author_dict = sorted(author_dict.items(), key=lambda x: x[1])
            authors = ", ".join([x[0] for x in author_dict])
            papers_return.append({
                "id": paper.id,
                "title": paper.title,
                "source": paper.source,
                "date": paper.date.strftime("%Y-%m-%d"),
                "level": mapper("paper_level", paper.level),
                "types": mapper("paper_types", paper.types),
                "authors": authors if len(authors) else "无"
            })
        return jsonify(papers_return)
    except Exception as e:
        print(e)
        return jsonify([])
    
# 0.查询某个论文的详细信息，包括作者json
@app.route('/get_single_paper/<paper_id>', methods=['GET'])
def get_single_paper(paper_id):
    try:
        paper = Paper.query.filter_by(id=paper_id).first()
        author_list = []
        # 查询论文作者
        authors = Publicate.query.filter_by(paper_id=paper.id).all()
        for author in authors:
            author_list.append({
                "teacher_id": author.teacher_id,
                "orders": author.orders,
                "teacher_is_ca": author.is_ca,
            })
        # 对作者按照orders排序
        author_list = sorted(author_list, key=lambda x: x["orders"])
        return jsonify({
            "id": paper.id,
            "title": paper.title,
            "source": paper.source,
            "date": paper.date.strftime("%Y-%m-%d"),
            "level": paper.level,
            "types": paper.types,
            "authors": author_list
        })
    except Exception as e:
        print(e)
        return jsonify([])
    
# 0.查询所有老师的姓名和工号对
@app.route('/get_all_teachers', methods=['GET'])
def get_all_teachers():
    try:
        teachers = Teacher.query.all()
        return jsonify([{
                'value': teacher.id,
                'label': teacher.name
            } for teacher in teachers])
    except Exception as e:
        print(e)
        return jsonify([])
    
# 0.查询所有课程的名字和课程号对
@app.route('/get_all_courses', methods=['GET'])
def get_all_courses():
    try:
        courses = Course.query.all()
        return jsonify([{
                'value': course.id,
                'label': course.name
            } for course in courses])
    except Exception as e:
        print(e)
        return jsonify([])

# 0.查询所有论文等级信息
@app.route('/get_all_paper_levels', methods=['GET'])
def get_all_paper_levels():
    try:
        return jsonify([{
                'value': paper_level,
                'label': mapper("paper_level", paper_level)
            } for paper_level in range(1, 7)])
    except Exception as e:
        print(e)
        return jsonify([])
    
# 0.查询所有论文类型信息
@app.route('/get_all_paper_types', methods=['GET'])
def get_all_paper_types():
    try:
        return jsonify([{
                'value': paper_type,
                'label': mapper("paper_types", paper_type)
            } for paper_type in range(1, 5)])
    except Exception as e:
        print(e)
        return jsonify([])
    
# 0.查询所有课程性质信息
@app.route('/get_all_course_property', methods=['GET'])
def get_all_course_property():
    try:
        return jsonify([{
                'value': course_property,
                'label': mapper("course_property", course_property)
            } for course_property in range(1, 3)])
    except Exception as e:
        print(e)
        return jsonify([])
    
# 0.查询所有课程学期信息
@app.route('/get_all_course_term', methods=['GET'])
def get_all_course_term():
    try:
        return jsonify([{
                'value': course_term,
                'label': mapper("course_term", course_term)
            } for course_term in range(1, 4)])
    except Exception as e:
        print(e)
        return jsonify([])
    
# 0.查询所有项目类型信息
@app.route('/get_all_project_types', methods=['GET'])
def get_all_project_types():
    try:
        return jsonify([{
                'value': project_type,
                'label': mapper("project_types", project_type)
            } for project_type in range(1, 6)])
    except Exception as e:
        print(e)
        return jsonify([])
    
# 0.查询所有职称信息
@app.route('/get_all_titles', methods=['GET'])
def get_all_titles():
    try:
        titles_return = []
        for title in range(1, 12):
            titles_return.append({
                'label': mapper("teacher_ranks", title)
            })
        return jsonify(titles_return)
    except Exception as e:
        print(e)
        return jsonify([])

# 1.登记发表论文情况-增
@app.route('/add_paper', methods=['POST'])
def add_paper():
    try:
        data = request.get_json()
        # 检查是否有必要的参数
        if ('isedit' not in data or
            (data['isedit'] and 'paper_id' not in data) or
            'paper_title' not in data or
            'paper_source' not in data or
            'paper_date' not in data or
            'paper_type' not in data or
            'paper_level' not in data or
            'paper_author' not in data):
            return jsonify({'result': 'failed', 'message': '缺少必要的参数'})
        # 检查论文类型和级别是否在约定的取值集合中选取
        if data['paper_type'] not in [1, 2, 3, 4] or data['paper_level'] not in [1, 2, 3, 4, 5, 6]:
            return jsonify({'result': 'failed', 'message': '非法的论文类型或级别'})
        if (not data['isedit']):
            paper = Paper(
                {
                    'title': data['paper_title'],
                    'source': data['paper_source'],
                    'date': data['paper_date'],
                    'types': data['paper_type'],
                    'level': data['paper_level']
                }
            )
            db.session.add(paper)
            db.session.commit()
            paper_id = paper.id
        else:
            paper_id = data['paper_id']
            paper = Paper.query.filter_by(id=paper_id).first()
            paper.title = data['paper_title']
            paper.source = data['paper_source']
            paper.date = data['paper_date']
            paper.types = data['paper_type']
            paper.level = data['paper_level']
            # 删除原来的所有publicate情况
            Publicate.query.filter_by(paper_id=paper_id).delete()
        # 添加相应的论文作者，因为数据库不做检验，所以这里也不做检验，检验留给前端做
        for author in data['paper_author']:
            publicate = Publicate(
                {
                    'teacher_id': author["teacher_id"],
                    'paper_id': paper_id,
                    'orders': author["teacher_orders"],
                    'is_ca': author["teacher_is_ca"]
                }
            )
            db.session.add(publicate)
        db.session.commit()
        return jsonify({'result': 'success', 'message': '增加一条发表论文情况成功'})
    except Exception as e:
        print(e)
        return jsonify({'result': 'failed', 'message': '由于内部错误，修改失败。'})

# 1.登记发表论文情况-删
#   直接删除有关联的publicate表中的记录，再从paper表中删除记录
@app.route('/delete_paper', methods=['POST'])
def delete_paper():
    try:
        data = request.get_json()
        # 检查是否有必要的参数
        if 'id' not in data:
            return jsonify({'result': 'failed', 'message': '缺少必要的参数'})
        # 检查论文id是否已经存在
        paper = Paper.query.filter_by(id=data['id']).first()
        if paper is None:
            return jsonify({'result': 'failed', 'message': '论文id不存在'})
        # 删除publicate表中的记录
        Publicate.query.filter_by(paper_id=data['id']).delete()
        # 删除paper表中的记录
        db.session.delete(paper)
        db.session.commit()
        return jsonify({'result': 'success', 'message': '删除一条发表论文情况成功'})
    except Exception as e:
        print(e)
        return jsonify({'result': 'failed', 'message': '由于内部错误，删除失败。'})

# 1.登记发表论文情况-查(按teacher_id)
@app.route('/get_publicate/<teacher_id>', methods=['GET'])
def get_publicate(teacher_id):
    try:
        # 检查是否有必要的参数
        if teacher_id is None:
            return jsonify({'result': 'failed', 'message': '缺少必要的参数'})
        # 和老师表关联查询
        result = db.session.query(Publicate, Paper).filter(
            Publicate.teacher_id == teacher_id,
            Publicate.paper_id == Paper.id
        ).all()
        # 查询是否有结果
        if len(result) == 0:
            return jsonify({'result': 'failed', 'message': '没有查询到发表论文情况'})
        # 查询老师的姓名
        teacher = Teacher.query.filter_by(id=teacher_id).first()
        teacher_info = {
            'teacher_name': teacher.name,
            'teacher_id': teacher.id,
            'teacher_sex': teacher.sex,
            'teacher_ranks': teacher.ranks,
        }
        # 拿到查询结果中的paper_title等
        publicate_list = []
        for item in result:
            publicate = item[0]
            paper = item[1]
            publicate_list.append({
                'year': paper.date.year,
                'paper_id': paper.id,
                'paper_title': paper.title,
                'paper_level': mapper("paper_level", paper.level),
                'paper_types': mapper("paper_types", paper.types),
                'paper_source': paper.source,
                'orders': publicate.orders,
                'is_ca': '通讯作者' if publicate.is_ca else '',
            })
        return jsonify({'result': 'success', 'message': '查询发表论文情况成功', 'publicate_list': publicate_list, 'teacher_info': teacher_info})
    except Exception as e:
        print(e)
        return jsonify({'result': 'failed', 'message': '由于内部错误，查询失败'})
    
# 2.查询所有项目信息
@app.route('/get_all_project', methods=['GET'])
def get_all_project():
    try:
        # 查询所有项目信息
        result = Project.query.all()
        # 查询是否有结果
        if len(result) == 0:
            return jsonify({'result': 'failed', 'message': '没有查询到项目信息。'})
        # 拿到查询结果中的项目信息等
        project_list = []
        for item in result:
            # 针对该项目，找出所有承担情况
            undertakes = Undertake.query.filter_by(project_id=item.id).all()
            undertakes_list = []
            for undertake in undertakes:
                teacher_name = Teacher.query.filter_by(id=undertake.teacher_id).first().name
                undertakes_list.append({
                    'teacher_id': undertake.teacher_id,
                    'teacher_name': teacher_name,
                    'orders': undertake.orders,
                    'undertake_money': undertake.undertake_money,
                })
            project_list.append({
                'id': item.id,
                'name': item.name,
                'source': item.source,
                'types': mapper("project_types", item.types),
                'money': item.money,
                'start_year': item.start_year,
                'end_year': item.end_year,
                'undertakes': undertakes_list,
            })
        return jsonify(project_list)
    except Exception as e:
        print(e)
        return jsonify([])

# 2.查询某个项目信息
@app.route('/get_project/<project_id>', methods=['GET'])
def get_project(project_id):
    try:
        # 查询项目信息
        item = Project.query.filter_by(id=project_id).all()
        # 查询是否有结果
        if len(item) == 0:
            return jsonify({'result': 'failed', 'message': '没有查询到项目信息。'})
        item = item[0]
        # 针对该项目，找出所有承担情况
        undertakes = Undertake.query.filter_by(project_id=item.id).all()
        undertakes_list = []
        for undertake in undertakes:
            teacher_name = Teacher.query.filter_by(id=undertake.teacher_id).first().name
            undertakes_list.append({
                'teacher_id': undertake.teacher_id,
                'teacher_name': teacher_name,
                'orders': undertake.orders,
                'undertake_money': undertake.undertake_money,
            })
        return_info = {
            'id': item.id,
            'name': item.name,
            'source': item.source,
            'types': item.types,
            'money': item.money,
            'start_year': item.start_year,
            'end_year': item.end_year,
            'undertakes': undertakes_list,
        }
        return jsonify(return_info)
    except Exception as e:
        print(e)
        return jsonify([])


# 2.登记承担项目情况
#   增加时，先登记项目情况，然后在下面直接登记老师情况
#   项目类型 只能在约定的取值集合中选取
@app.route('/add_project', methods=['POST'])
def add_project():
    """
    增加一条项目情况
    data:
        id: 项目id
        name: 项目名称
        source: 项目来源
        types: 项目类型
        start_year: 项目开始时间
        end_year: 项目结束时间
        teachers: 一个列表，每个元素是一个字典，包含以下信息
            teacher_id: 教师id
            orders: 教师排名
            undertake_money: 教师承担金额
    """
    try:
        data = request.get_json()
        # 检查是否有必要的参数
        if ("id" not in data or 
            "name" not in data or 
            "source" not in data or 
            "types" not in data or 
            "start_year" not in data or 
            "end_year" not in data):
            return jsonify({'result': 'failed', 'message': '缺少必要的参数'})
        # 检查项目id是否已经存在
        # isedit: True 要求项目id必须存在
        # isedit: False 要求项目id必须不存在
        id_find = Project.query.filter_by(id=data['id']).first()
        if (id_find is None) and (data['isedit'] is True):
            return jsonify({'result': 'failed', 'message': '项目id不存在'})
        if (id_find is not None) and (data['isedit'] is False):
            return jsonify({'result': 'failed', 'message': '项目id已经存在'})
        # 检查项目类型是否合法
        if data['types'] not in [1, 2, 3, 4, 5]:
            return jsonify({'result': 'failed', 'message': '项目类型不合法'})
        teachers = data['undertakes']
        total_money = 0
        for teacher in teachers:
            # 检查承担金额是否为空
            if teacher['undertake_money'] is None:
                return jsonify({'result': 'failed', 'message': '承担金额不能为空'})
            # 检查承担金额是否合法
            if teacher['undertake_money'] <= 0:
                return jsonify({'result': 'failed', 'message': '承担金额不合法'})
            total_money += teacher['undertake_money']
        # 为了简单起见，这里不检查时间、排名、教师id是否合法，这些都在前端做了
        if not data['isedit']:
            project = Project(
                {
                    "id": data['id'],
                    "name": data['name'],
                    "source": data['source'],
                    "types": data['types'],
                    "money": total_money,
                    "start_year": data['start_year'],
                    "end_year": data['end_year']
                }
            )
            db.session.add(project)
            db.session.commit()
        else:
            project = Project.query.filter_by(id=data['id']).first()
            project.name = data['name']
            project.source = data['source']
            project.types = data['types']
            project.money = total_money
            project.start_year = data['start_year']
            project.end_year = data['end_year']
            # 删除原来的承担情况
            Undertake.query.filter_by(project_id=data['id']).delete()
        
        for teacher in teachers:
            undertake = Undertake(
                {
                    "teacher_id": teacher['teacher_id'],
                    "project_id": data['id'],
                    "orders": teacher['orders'],
                    "undertake_money": teacher['undertake_money']
                }
            )
            db.session.add(undertake)
        db.session.commit()
        return jsonify({'result': 'success', 'message': '增加一条项目情况成功'})
    except Exception as e:
        print(e)
        return jsonify({'result': 'failed', 'message': '增加一条项目情况失败'})

# 2.删除项目
@app.route('/delete_project', methods=['POST'])
def delete_project():
    try:
        data = request.get_json()
        # 检查是否有必要的参数
        if "project_id" not in data:
            return jsonify({'result': 'failed', 'message': '缺少必要的参数'})
        # 检查项目id是否已经存在
        id_find = Project.query.filter_by(id=data['project_id']).first()
        if id_find is None:
            return jsonify({'result': 'failed', 'message': '项目id不存在'})
        # 删除承担情况
        Undertake.query.filter_by(project_id=data['project_id']).delete()
        db.session.delete(id_find)
        db.session.commit()
        return jsonify({'result': 'success', 'message': '删除一条项目情况成功'})
    except Exception as e:
        print(e)
        return jsonify({'result': 'failed', 'message': '删除一条项目情况失败'})

# 3.提交course_id，然后提供在所有年、学期的教学记录
@app.route('/get_teach_infos/<course_id>', methods=['GET'])
def get_teach_infos(course_id):
    try:
        item = Course.query.filter_by(id=course_id).first()
        if item is None:
            return jsonify({'result': 'failed', 'message': '课程id不存在'})
        return_infos = {
            'id': item.id,
            'name': item.name,
            'hours': item.hours,
            'property': item.property,
            'teach_info': []
        }
        # 查询所有该课程有记录的年份year和学期term
        teach_record = db.session.query(Teach.year, Teach.term).filter_by(course_id=course_id).distinct().group_by(Teach.term, Teach.year).all()
        for record in teach_record:
            new_info = {
                'year': record.year,
                'term': record.term,
                'teachers': []
            }
            # 查询每个年份、学期的教师信息  
            teachers = db.session.query(Teach.teacher_id, Teach.hours).filter_by(course_id=course_id, year=record.year, term=record.term).all()
            for teacher in teachers:
                # 找到名字
                teacher_name = Teacher.query.filter_by(id=teacher.teacher_id).first().name
                new_info['teachers'].append({
                    'teacher_id': teacher.teacher_id,
                    'teacher_name': teacher_name,
                    'teach_time': teacher.hours
                })
            return_infos['teach_info'].append(new_info)
        return jsonify(return_infos)
    except Exception as e:
        print(e)
        return jsonify([])

# 3.提交增加的教学记录
@app.route('/add_teach_info', methods=['POST'])
def add_teach_info():
    try:
        data = request.get_json()
        # 检查是否有必要的参数
        if 'isedit' not in data or 'formdata' not in data:
            return jsonify({'result': 'failed', 'message': '缺少必要的参数'})
        isedit = data['isedit']
        data = data['formdata']
        # 检查formdata中的字段是否都存在
        if 'id' not in data or 'year' not in data or 'term' not in data or 'teachers' not in data:
            return jsonify({'result': 'failed', 'message': '缺少必要的参数'})
        # 检查是否添加了老师
        if len(data['teachers']) == 0:
            return jsonify({'result': 'failed', 'message': '至少添加一位教师'})
        # 检查课程id是否已经存在
        id_find = Course.query.filter_by(id=data['id']).first()
        if id_find is None:
            return jsonify({'result': 'failed', 'message': '课程id不存在'})
        # 检查年份是否合法
        if data['year'] < 2000 or data['year'] > 2100:
            return jsonify({'result': 'failed', 'message': '年份不合法'})
        # 检查学期是否合法
        if data['term'] not in [1, 2]:
            return jsonify({'result': 'failed', 'message': '学期不合法'})
        teachers = data['teachers']
        total_hours = 0
        for teacher in teachers:
            # 检查教师id是否存在
            teacher_find = Teacher.query.filter_by(id=teacher['teacher_id']).first()
            if teacher_find is None:
                return jsonify({'result': 'failed', 'message': '教师id不存在'})
            # 检查授课时间是否为空
            if teacher['teach_time'] is None or teacher['teach_time'] <= 0:
                return jsonify({'result': 'failed', 'message': '授课时间不合法'})
            total_hours += teacher['teach_time']
        # 检查总授课时间是否等于课程总学时
        if total_hours != id_find.hours:
            return jsonify({'result': 'failed', 'message': '总授课时间与课程总学时不符'})
        # 检查是否已经存在该条记录
        if isedit:
            # 删除原来的
            Teach.query.filter_by(course_id=data['id'], year=data['year'], term=data['term']).delete()
        # 插入新的
        for teacher in teachers:
            if not isedit:
                # 检查是否这个老师已经有该课程的授课记录
                teach_find = Teach.query.filter_by(course_id=data['id'], teacher_id=teacher['teacher_id']).first()
                if teach_find is not None:
                    return jsonify({'result': 'failed', 'message': '这位老师已经上过这门课了'})
            teach = Teach(
                {
                    "teacher_id": teacher['teacher_id'],
                    "course_id": data['id'],
                    "year": data['year'],
                    "term": data['term'],
                    "hours": teacher['teach_time']
                }
            )
            db.session.add(teach)
        db.session.commit()
        return jsonify({'result': 'success', 'message': '增加一条教学记录成功'})
    except Exception as e:
        print(e)
        return jsonify({'result': 'failed', 'message': '未知错误'})

# 3.删除教学记录
@app.route('/delete_teach_info', methods=['POST'])
def delete_teach_info():
    try:
        data = request.get_json()
        # 检查是否有必要的参数
        if 'course_id' not in data or 'year' not in data or 'term' not in data:
            return jsonify({'result': 'failed', 'message': '缺少必要的参数'})
        course_id = data['course_id']
        year = data['year']
        term = data['term']
        # 检查课程id是否已经存在
        id_find = Course.query.filter_by(id=course_id).first()
        if id_find is None:
            return jsonify({'result': 'failed', 'message': '课程id不存在'})
        # 检查年份是否合法
        if year < 2000 or year > 2100:
            return jsonify({'result': 'failed', 'message': '年份不合法'})
        # 检查学期是否合法
        if term not in [1, 2]:
            return jsonify({'result': 'failed', 'message': '学期不合法'})
        # 删除记录
        Teach.query.filter_by(course_id=course_id, year=year, term=term).delete()
        db.session.commit()
        return jsonify({'result': 'success', 'message': '删除一条教学记录成功'})
    except Exception as e:
        print(e)
        return jsonify({'result': 'failed', 'message': '未知错误'})

# 4.按教师工号和给定年份范围查询教师教学科研情况
@app.route('/get_teacher/<teacher_id>/<year_start>/<year_end>', methods=['GET'])
def get_teacher(teacher_id, year_start, year_end):
    return_infos = {
        'basic_info': {
            'teacher_id': teacher_id,
            'teacher_name': "",
            'teacher_sex': "",
            'teacher_ranks': "",
        },
        'teach_info': [],
        'research_info': [],
        'undertake_info': []
    }
    try:
        # 检查教师id是否存在
        teacher_find = Teacher.query.filter_by(id=teacher_id).first()
        if teacher_find is None:
            return jsonify({'result': 'failed', 'message': '教师id不存在'})
        # 填写教师信息
        return_infos['basic_info']['teacher_name'] = teacher_find.name
        return_infos['basic_info']['teacher_sex'] = mapper("sex", teacher_find.sex)
        return_infos['basic_info']['teacher_ranks'] = mapper("teacher_ranks", teacher_find.ranks)
        # 检查年份是否合法
        if int(year_start) < 2000 or int(year_start) > 2100 or int(year_end) < 2000 or int(year_end) > 2100 or int(year_start) > int(year_end):
            return jsonify({'result': 'failed', 'message': '年份不合法'})
        # 填写教学信息
        teach_find = Teach.query.filter_by(teacher_id=teacher_id).all()
        for teach in teach_find:
            if teach.year >= int(year_start) and teach.year <= int(year_end):
                course_find = Course.query.filter_by(id=teach.course_id).first()
                if course_find is not None:
                    return_infos['teach_info'].append({
                        'course_id': course_find.id,
                        'course_name': course_find.name,
                        'teach_time': teach.hours,
                        'year_term': str(teach.year) + ' ' + mapper("course_term", teach.term)
                    })
        # 填写科研信息
        publicate_find = Publicate.query.filter_by(teacher_id=teacher_id).all()
        for publicate in publicate_find:
            paper_find = Paper.query.filter_by(id=publicate.paper_id).first()
            if paper_find is not None:
                if paper_find.date.year >= int(year_start) and paper_find.date.year <= int(year_end):
                    return_infos['research_info'].append({
                        'paper_title': paper_find.title,
                        'paper_source': paper_find.source,
                        'paper_year': paper_find.date.year,
                        'paper_types': mapper("paper_level", paper_find.level),
                        'paper_orders': publicate.orders,
                        'paper_is_ca': "通讯作者" if publicate.is_ca else None
                    })
        # 填写承担项目信息
        undertake_find = Undertake.query.filter_by(teacher_id=teacher_id).all()
        for undertake in undertake_find:
            project_find = Project.query.filter_by(id=undertake.project_id).first()
            if project_find is not None:
                if project_find.start_year <= int(year_start) and project_find.end_year >= int(year_end):
                    return_infos['undertake_info'].append({
                        'project_name': project_find.name,
                        'project_source': project_find.source,
                        'project_types': mapper("project_types", project_find.types),
                        'project_start_end': str(project_find.start_year) + '-' + str(project_find.end_year),
                        'project_money': project_find.money,
                        'project_undertake_money': undertake.undertake_money
                    })
        return jsonify({'result': 'success', 'message': '查询成功', 'data': return_infos})
    except Exception as e:
        print(e)
        return jsonify({'result': 'failed', 'message': '未知错误'})

app.run()
